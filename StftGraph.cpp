#include "StftGraph.h"

#include "RootUtilities.h"
#include "StftAlgorithm.h"

/// ROOT
#include "TCanvas.h"
#include "TH2F.h"

namespace Visualisation
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StftGraph::StftGraph( const WaveAnalysis::StftAlgorithm& fftwAlg ) :
   m_stftAlg( fftwAlg ),
   m_canvas( 0 ),
   m_hist( 0 )
{
}

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
   if ( !m_stftAlg.hasExecuted() || m_stftAlg.getNumSpectra() == 0 )
   {
      throw ExceptionDataNotPrepared( "StftGraph", "StftAlgorithm data" );
   }

   size_t nBinsX = m_stftAlg.getNumSpectra();
   size_t nBinsY = m_stftAlg.getSpectrum( 0 ).size();

   m_canvas = new TCanvas();

   /// TODO: scale according to samples/frequencies
   double xMin = -0.5;
   double xMax = nBinsX - 0.5;

   double yMin = -0.5;
   double yMax = nBinsY - 0.5;

   TString uniqueName = RootUtilities::getInstance().generateUniqueName( "StftGraph" );

   m_hist = new TH2F( uniqueName, uniqueName, nBinsX, xMin, xMax, nBinsY, yMin, yMax );

   for ( size_t iX = 0; iX < nBinsX; ++iX )
   {
      const WaveAnalysis::FourierSpectrum& spec = m_stftAlg.getSpectrum( iX );
      for ( size_t iY = 0; iY < nBinsY; ++iY )
      {
         m_hist->SetBinContent( iX + 1, iY + 1, spec.getMagnitudeInBin( iY ) );
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
