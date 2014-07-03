#include "StftGraph.h"

#include "IPlotFactory.h"
#include "Regular2DHistogram.h"
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
StftGraph::StftGraph( const WaveAnalysis::StftData& stftData ) :
   m_stftData( stftData ),
   m_hist( 0 )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StftGraph::~StftGraph()
{
   delete m_hist;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// create
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void StftGraph::create( const std::string& name )
{
   if ( m_stftData.getNumSpectra() == 0 )
   {
      throw ExceptionDataNotPrepared( "StftGraph", "StftAlgorithm data" );
   }

   size_t nBinsX = m_stftData.getNumSpectra();
   size_t nBinsY = m_stftData.getConfig().getSpectrumDimension();

   /// TODO: scale according to samples/frequencies
   double xMin = -0.5;
   double xMax = nBinsX - 0.5;

   double yMin = -0.5;
   double yMax = m_stftData.getConfig().getSamplingInfo().getNyquistFrequency() + 0.5;

   m_hist = new Math::Regular2DHistogram( nBinsX, xMin, xMax, nBinsY, yMin, yMax );

   for ( size_t iX = 0; iX < nBinsX; ++iX )
   {
      size_t specIndex = static_cast< double >( iX ) / nBinsX * m_stftData.getNumSpectra();
      const WaveAnalysis::FourierSpectrum& spec = m_stftData.getSpectrum( specIndex );
      for ( size_t iY = 0; iY < m_stftData.getConfig().getSpectrumDimension(); ++iY )
      {
         m_hist->setBinContent( iX, iY, spec.getMagnitudeInBin( iY ) );
      }
   }

   gPlotFactory().createPlot( name );
   gPlotFactory().create2DHist( *m_hist, Plotting::Palette::heatPalette() );

}


} /// namespace Visualisation
