#include "RebinnedSRGraph.h"

#include "Exceptions.h"
#include "IPlotFactory.h"
#include "Logger.h"
#include "Regular2DHistogram.h"
#include "SrSpectrum.h"
#include "StftData.h"

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

   double freqBinWidth = stftData.getConfig().getSpectrumFrequencies()[1] - stftData.getConfig().getSpectrumFrequencies()[0];
   double minY = stftData.getConfig().getSpectrumFrequencies()[0] - 0.5 * freqBinWidth;
   double maxY = stftData.getConfig().getSpectrumFrequencies().back() - 0.5 * freqBinWidth;

   m_hist = new Math::Regular2DHistogram( nSpectra, -0.5, nSpectra - 0.5, nFreq, minY, maxY );
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
   Logger msg( "RebinnedSRGraph" );
   msg << Msg::Verbose << "In create" << Msg::EndReq;

   if ( m_stftData.getNumSpectra() == 0 )
   {
      throw ExceptionDataNotPrepared( "StftGraph", "StftAlgorithm data" );
   }
   assert( dynamic_cast< const WaveAnalysis::SrSpectrum* >( &m_stftData.getSpectrum( 0 ) ) );

   for ( size_t iX = 0; iX < m_hist->getNumBinsX(); ++iX )
   {
      const WaveAnalysis::SrSpectrum& srSpec = static_cast< const WaveAnalysis::SrSpectrum& >( m_stftData.getSpectrum( iX ) );
      for ( size_t iY = 0; iY < m_hist->getNumBinsY(); ++iY )
      {
         double freq = srSpec.getFrequencies()[ iY ];
         double val  = srSpec.getMagnitudeInBin( iY );
         m_hist->add( iX, freq, val );
      }
   }

   gPlotFactory().createPlot( name );
   gPlotFactory().create2DHist( *m_hist, Plotting::Palette::heatPalette() );

   msg << Msg::Verbose << "Done" << Msg::EndReq;
}

} /// namespace Visualisation
