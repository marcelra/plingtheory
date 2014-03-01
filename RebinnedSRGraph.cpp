#include "RebinnedSRGraph.h"

#include "Exceptions.h"
#include "Logger.h"
#include "StftData.h"
#include "RootUtilities.h"
#include "SrSpectrum.h"

#include "TCanvas.h"
#include "TColor.h"
#include "TH2F.h"

namespace Visualisation
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RebinnedSRGraph::RebinnedSRGraph( const WaveAnalysis::StftData& stftData, size_t nBinsX, size_t nBinsY ) :
   m_stftData( stftData ),
   m_canvas( 0 ),
   m_hist( 0 ),
   m_nBinsX( nBinsX == 0 ? stftData.getNumSpectra() : nBinsX ),
   m_nBinsY( nBinsY == 0 ? m_stftData.getConfig().getSpectrumDimension() : nBinsY )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RebinnedSRGraph::~RebinnedSRGraph()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// create
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TCanvas* RebinnedSRGraph::create()
{
   Logger msg( "RebinnedSRGraph" );
   msg << Msg::Verbose << "In create" << Msg::EndReq;

   /// Cannot make plot twice
   assert( m_canvas == 0 );
   if ( m_stftData.getNumSpectra() == 0 )
   {
      throw ExceptionDataNotPrepared( "StftGraph", "StftAlgorithm data" );
   }
   assert( dynamic_cast< const WaveAnalysis::SrSpectrum* >( &m_stftData.getSpectrum( 0 ) ) );

   m_canvas = new TCanvas();

   TString uniqueName = RootUtilities::getInstance().generateUniqueName( "StftGraph" );
   m_hist = new TH2F( uniqueName, uniqueName, m_nBinsX, -0.5, m_nBinsX - 0.5, m_nBinsY, -0.5, m_nBinsY - 0.5 );

   double stops[2] = { 0, 1 };
   double red[2] = { 1, 0 };
   double green[2] = { 1, 0 };
   double blue[2] = { 1, 0 };
   TColor::CreateGradientColorTable( 2, stops, red, green, blue, 255, 1 );

   for ( size_t iX = 0; iX < m_nBinsX; ++iX )
   {
      const WaveAnalysis::SrSpectrum& srSpec = static_cast< const WaveAnalysis::SrSpectrum& >( m_stftData.getSpectrum( iX ) );
      const Math::RegularAccumArray& binnedSrSpec = srSpec.rebinToFourierLattice();
      for ( size_t iY = 0; iY < m_nBinsY; ++iY )
      {
         double val = binnedSrSpec.getBinContent( iY );
         m_hist->SetBinContent( iX + 1, iY + 1, val );
      }
   }

   m_hist->Draw( "COLZ" );
   msg << Msg::Verbose << "Done" << Msg::EndReq;

   return m_canvas;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getCanvas
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const TCanvas* RebinnedSRGraph::getCanvas() const
{
   return m_canvas;
}

TCanvas* RebinnedSRGraph::getCanvas()
{
   return const_cast< TCanvas* >( static_cast< const RebinnedSRGraph* >( this )->getCanvas() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getGraph
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const TH2F* RebinnedSRGraph::getGraph() const
{
   return m_hist;
}

TH2F* RebinnedSRGraph::getGraph()
{
   return const_cast< TH2F* >( static_cast< const RebinnedSRGraph* >( this )->getGraph() );
}


} /// namespace Visualisation
