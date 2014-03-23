#include "RootPlotFactory.h"

#include "Exceptions.h"
#include "IAccumArray.h"
#include "Logger.h"
#include "RegularAccumArray.h"
#include "RootUtilities.h"
#include "StftGraph.h"

/// ROOT includes
#include "TCanvas.h"
#include "TColor.h"
#include "TList.h"

#include <cassert>

namespace PlotInterface
{

RootPlotFactory::RootPlotFactory() :
   IPlotFactory( "RootPlotFactory" ),
   m_currentCanvas( 0 )
{}

RootPlotFactory::~RootPlotFactory()
{}

void RootPlotFactory::initialise()
{
   assert( IPlotFactory::s_instance == 0 );
   s_instance  = new RootPlotFactory();
}

void RootPlotFactory::createPlot( const std::string& name )
{
   TString canvasName = RootUtilities::getInstance().generateUniqueName( "auto_plot_canvas" );
   TCanvas* canvas = new TCanvas( canvasName, name.c_str() );
   m_plots[ name ] = canvas;
   // canvas->cd();
   m_currentCanvas = canvas;
}

void RootPlotFactory::createGraph( const std::vector< double >& xData, const std::vector< double >& yData, const QColor& colour )
{
   if ( !m_currentCanvas )
   {
      throw ExceptionNoPlotAvailable();
   }

   TGraph* graph = RootUtilities::createGraph( xData, yData );
   graph->SetLineColor( rootColorFromQColor( colour ) );

   if ( checkIsNewCanvas() )
   {
      graph->Draw( "AL" );
   }
   else
   {
      graph->Draw( "LSAME" );
   }
}

void RootPlotFactory::drawPcmData( const RawPcmData& pcmData, const QColor& colour )
{
   if ( !m_currentCanvas )
   {
      throw ExceptionNoPlotAvailable();
   }

   TGraph* graph = RootUtilities::createGraph( pcmData );
   graph->SetLineColor( TColor::GetColor( rootColorFromQColor( colour ) ) );

   if ( checkIsNewCanvas() )
   {
      graph->Draw( "AL" );
   }
   else
   {
      graph->Draw( "LSAME" );
   }
}

bool RootPlotFactory::checkIsNewCanvas()
{
   assert( m_currentCanvas );
   if ( m_currentCanvas->GetListOfPrimitives()->GetSize() == 0 )
   {
      return true;
   }
   if ( m_currentCanvas->GetListOfPrimitives()->At( 0 )->ClassName() == TString( "TFrame" ) )
   {
      return false;
   }
   // getLogger() << Msg::Warning << "First primitive on ROOT canvas is not of class TFrame instead it is " << m_currentCanvas->GetListOfPrimitives()->At( 0 )->ClassName() << Msg::EndReq;
   return false;
}

void RootPlotFactory::createScatter( const std::vector< double >& xData, const std::vector< double >& yData, const QColor& colour )
{
   if ( !m_currentCanvas )
   {
      throw ExceptionNoPlotAvailable();
   }

   TGraph* graph = RootUtilities::createGraph( xData, yData );
   graph->SetLineColor( rootColorFromQColor( colour ) );

   if ( checkIsNewCanvas() )
   {
      graph->Draw( "AP" );
   }
   else
   {
      graph->Draw( "PSAME" );
   }
}

int RootPlotFactory::rootColorFromQColor( const QColor& colour )
{
   return TColor::GetColor( (Float_t) colour.redF(), colour.greenF(), colour.blueF() );
}

void RootPlotFactory::createStftGraph( const WaveAnalysis::StftData& stftData )
{
   Visualisation::StftGraph stftGraph( stftData );
   m_currentCanvas = stftGraph.create();
}

void RootPlotFactory::createHistogram( const Math::IAccumArray& hist, const QColor& colour )
{
   if ( !m_currentCanvas )
   {
      throw ExceptionNoPlotAvailable();
   }

   const Math::RegularAccumArray* regularAccumArray = dynamic_cast< const Math::RegularAccumArray* >( &hist );
   assert( regularAccumArray );

   RootUtilities& ru = RootUtilities::getInstance();
   TString uniqueName = ru.generateUniqueName( "hist" );

   TH1F* rootHist = new TH1F( uniqueName, uniqueName, hist.getNumBins(), hist.getMinX(), hist.getMaxX() );
   for ( size_t i = 0; i < hist.getNumBins(); ++i )
   {
      rootHist->SetBinContent( i + 1, hist.getBinContent( i ) );
   }

   rootHist->SetBinContent( 0, hist.getUnderflow() );
   rootHist->SetBinContent( hist.getNumBins(), hist.getOverflow() );
   rootHist->SetLineColor( rootColorFromQColor( colour ) );

   if ( checkIsNewCanvas() )
   {
      rootHist->Draw();
   }
   else
   {
      rootHist->Draw( "SAME" );
   }
}

} /// namespace PlotInterface