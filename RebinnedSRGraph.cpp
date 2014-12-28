#include "RebinnedSRGraph.h"

#include "Exceptions.h"
#include "IPlotFactory.h"
#include "Logger.h"
#include "Regular2DHistogram.h"
#include "SrSpectrum.h"
#include "StftData.h"
#include "WindowLocation.h"

namespace Visualisation
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RebinnedSRGraph::RebinnedSRGraph( const WaveAnalysis::StftData& stftData ) :
   m_stftData( stftData )
{
   size_t nSpectra = stftData.getNumSpectra();
   size_t nFreq    = stftData.getConfig().getSpectrumDimension();

   double minX = stftData.getWindowLocation( 0 ).getFirstSample();
   double maxX = stftData.getWindowLocation( nSpectra - 1 ).getLastSample();

   double freqBinWidth = stftData.getConfig().getSpectrumFrequencies()[1] - stftData.getConfig().getSpectrumFrequencies()[0];
   double minY = stftData.getConfig().getSpectrumFrequencies()[0] - 0.5 * freqBinWidth;
   double maxY = stftData.getConfig().getSpectrumFrequencies().back() - 0.5 * freqBinWidth;

   m_hist = new Math::Regular2DHistogram( nSpectra, minX, maxX, nFreq, minY, maxY );

   if ( m_stftData.getNumSpectra() == 0 )
   {
      throw ExceptionDataNotPrepared( "StftGraph", "StftAlgorithm data" );
   }
   assert( dynamic_cast< const WaveAnalysis::SrSpectrum* >( &m_stftData.getSpectrum( 0 ) ) );

   for ( size_t iX = 0; iX < m_hist->getNumBinsX(); ++iX )
   {

      const WaveAnalysis::SrSpectrum& srSpec = static_cast< const WaveAnalysis::SrSpectrum& >( m_stftData.getSpectrum( iX ) );
      size_t binCentreX = srSpec.getWindowLocation()->getCentre();

      for ( size_t iY = 0; iY < m_hist->getNumBinsY(); ++iY )
      {
         double freq = srSpec.getFrequencies()[ iY ];
         double val  = srSpec.getMagnitudeInBin( iY );
         m_hist->add( binCentreX, freq, val );
      }
   }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RebinnedSRGraph::~RebinnedSRGraph()
{
   delete m_hist;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// create
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RebinnedSRGraph::create( const std::string& name )
{
   gPlotFactory().createPlot( name );
   gPlotFactory().create2DHist( *m_hist, Plotting::Palette::heatPalette() );
}

} /// namespace Visualisation
