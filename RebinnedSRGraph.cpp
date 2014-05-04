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
RebinnedSRGraph::RebinnedSRGraph( const WaveAnalysis::StftData& stftData, size_t nBinsX, size_t nBinsY ) :
   m_stftData( stftData ),
   m_hist( 0 ),
   m_nBinsX( nBinsX == 0 ? stftData.getNumSpectra() : nBinsX ),
   m_nBinsY( nBinsY == 0 ? m_stftData.getConfig().getSpectrumDimension() : nBinsY )
{}

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

   /// Cannot make plot twice
   assert( m_hist == 0 );

   if ( m_stftData.getNumSpectra() == 0 )
   {
      throw ExceptionDataNotPrepared( "StftGraph", "StftAlgorithm data" );
   }
   assert( dynamic_cast< const WaveAnalysis::SrSpectrum* >( &m_stftData.getSpectrum( 0 ) ) );

   m_hist = new Math::Regular2DHistogram( m_nBinsX, -0.5, m_nBinsX - 0.5, m_nBinsY, -0.5, m_nBinsY - 0.5 );

   for ( size_t iX = 0; iX < m_nBinsX; ++iX )
   {
      std::cout << "iX = " << iX << ", m_nBinsX = " << m_nBinsX << std::endl;
      const WaveAnalysis::SrSpectrum& srSpec = static_cast< const WaveAnalysis::SrSpectrum& >( m_stftData.getSpectrum( iX ) );
      const Math::RegularAccumArray& binnedSrSpec = srSpec.rebinToFourierLattice();
      for ( size_t iY = 0; iY < m_nBinsY; ++iY )
      {
         double val = binnedSrSpec.getBinContent( iY );
         m_hist->setBinContent( iX, iY, val );
      }
   }

   gPlotFactory().createPlot( name );
   gPlotFactory().create2DHist( *m_hist );

   msg << Msg::Verbose << "Done" << Msg::EndReq;
}

} /// namespace Visualisation
