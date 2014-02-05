#include "StftGraph.h"

#include "RootUtilities.h"
#include "StftAlgorithm.h"

/// ROOT
#include "TCanvas.h"
#include "TColor.h"
#include "TH2F.h"

namespace Visualisation
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StftGraph::StftGraph( const WaveAnalysis::RawStftData& stftData, size_t nBinsX, size_t nBinsY ) :
   m_stftData( stftData ),
   m_canvas( 0 ),
   m_hist( 0 ),
   m_nBinsX( nBinsX == 0 ? stftData.getNumSpectra() : nBinsX ),
   m_nBinsY( nBinsY == 0 ? m_stftData.getConfig().getSpectrumDimension() : nBinsY )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StftGraph::~StftGraph()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// create
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TCanvas* StftGraph::create()
{
   assert( m_canvas == 0 );
   if ( m_stftData.getNumSpectra() == 0 )
   {
      throw ExceptionDataNotPrepared( "StftGraph", "StftAlgorithm data" );
   }

   m_canvas = new TCanvas();

   /// TODO: scale according to samples/frequencies
   double xMin = -0.5;
   double xMax = m_nBinsX - 0.5;

   double yMin = -0.5;
   double yMax = m_stftData.getConfig().getSamplingInfo().getNyquistFrequency() + 0.5;

   TString uniqueName = RootUtilities::getInstance().generateUniqueName( "StftGraph" );

   double stops[2] = { 0, 1 };
   double red[2] = { 1, 0 };
   double green[2] = { 1, 0 };
   double blue[2] = { 1, 0 };
   TColor::CreateGradientColorTable( 2, stops, red, green, blue, 255, 1 );

   m_hist = new TH2F( uniqueName, uniqueName, m_nBinsX, xMin, xMax, m_nBinsY, yMin, yMax );

   for ( size_t iX = 0; iX < m_nBinsX; ++iX )
   {
      size_t specIndex = static_cast< double >( iX ) / m_nBinsX * m_stftData.getNumSpectra();
      const WaveAnalysis::FourierSpectrum& spec = m_stftData.getSpectrum( specIndex );
      // for ( size_t iBinY = 0; iBinY < m_nBinsY; ++iBinY )
      // {
      //    m_hist->SetBinContent( iX + 1, iBinY + 1, 1e-100 );
      // }
      for ( size_t iY = 0; iY < m_stftData.getConfig().getSpectrumDimension(); ++iY )
      {
         // size_t binY = m_hist->GetYaxis()->FindBin( spec.getFrequencyOfBin(iY) ) + 1;
         // m_hist->SetBinContent( iX + 1, binY, spec.getMagnitudeInBin( iY ) );
         m_hist->Fill( iX + 1, spec.getFrequencyOfBin( iY ), spec.getMagnitudeInBin( iY ) );
      }
   }

   m_hist->Draw( "colz" );
   return m_canvas;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getAveragingWeights
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// StftGraph::WeightMap StftGraph::getAveragingWeights( double xMin, double xMax ) const
// {
//    /// TODO: figure out if this can be improved
//    WeightMap wMap;
//    size_t indexMin = floor( xMin );
//    size_t indexMax = floor( xMax + 1 );
//    double firstWeight = indexMin + 1 - xMin;
//    double sumWeights = firstWeight;
//    wMap.insert( std::pair< int, double >( indexMin, firstWeight ) );
//    for ( size_t index = indexMin + 1; index < indexMax - 1; ++index )
//    {
//       sumWeights += 1;
//       wMap.insert( std::pair< int, double >( index, 1 ) );
//    }
//    double lastWeight = xMax - indexMax;
//    if ( lastWeight > 0 )
//    {
//       wMap.insert( std::pair< int, double >( indexMax, xMax - indexMax ) );
//       sumWeights += lastWeight;
//    }
//    WeightMap::iterator it = wMap.begin();
//    for ( ; it != wMap.end(); ++it )
//    {
//       it->second /= sumWeights;
//    }
//    return wMap;
// }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// calcAveragedSpectrum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RealVector StftGraph::calcAveragedSpectrum( const WeightMap& weightMap ) const
// {
//    WeightMap::const_iterator it = weightMap.begin();
//    RealVector result = m_fftwAlg.getSpectrum( it->first ).getMagnitude() * it->second;
//    ++it;
//    for ( ; it != weightMap.end(); ++it )
//    {
//       const RealVector& spec = m_fftwAlg.getSpectrum( it->first ).getMagnitude();
//       for ( size_t iFreq = 0; iFreq < result.size(); ++iFreq )
//       {
//          result[ iFreq ] += spec[ iFreq ] * it->second;
//       }
//    }
//    return result;
// }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getCanvas
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const TCanvas* StftGraph::getCanvas() const
{
   return m_canvas;
}

TCanvas* StftGraph::getCanvas()
{
   return const_cast< TCanvas* >( static_cast< const StftGraph* >( this )->getCanvas() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getGraph
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const TH2F* StftGraph::getGraph() const
{
   return m_hist;
}

TH2F* StftGraph::getGraph()
{
   return const_cast< TH2F* >( static_cast< const StftGraph* >( this )->getGraph() );
}

} /// namespace Visualisation
