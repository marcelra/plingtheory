#include "QtPlotFactory.h"

#include "AvailablePlotsList.h"
#include "CurveItem.h"
#include "Exceptions.h"
#include "Hist2DItem.h"
#include "HistogramItem.h"
#include "IAccumArray.h"
#include "Palette.h"
#include "Plot2D.h"
#include "RawPcmData.h"
#include "RegLargeDataCurve.h"
#include "ScatterItem.h"
#include "StftPaintItem.h"
#include "Utils.h"
#include "ZScatterItem.h"

#include <cassert>

namespace PlotInterface
{

QtPlotFactory::QtPlotFactory() :
   IPlotFactory( "QtPlotFactory" )
{}

QtPlotFactory::~QtPlotFactory()
{
   for ( size_t i = 0; i < m_plotItems.size(); ++i )
   {
      delete m_plotItems[ i ];
   }
}

void QtPlotFactory::initialise()
{
   assert( IPlotFactory::s_instance == 0 );
   s_instance  = new QtPlotFactory();
}

void QtPlotFactory::createPlot( const std::string& name )
{
   Gui::AvailablePlotsList& plotsList = Gui::AvailablePlotsList::getInstance();
   Plotting::Plot2D* plot2D = plotsList.addPlot( name );
   m_currentPlot = plot2D;
}

void QtPlotFactory::createGraph( const std::vector< double >& xData, const std::vector< double >& yData, const QColor& colour )
{
   if ( !m_currentPlot )
   {
      throw ExceptionNoPlotAvailable();
   }

   Plotting::CurveItem* item = new Plotting::CurveItem( xData, yData );
   item->setLineColor( colour );
   item->setAntialiasing( true );
   m_currentPlot->addItem( item );
   m_plotItems.push_back( item );
}

void QtPlotFactory::drawPcmData( const RawPcmData& pcmData, const QColor& colour )
{
   if ( !m_currentPlot )
   {
      throw ExceptionNoPlotAvailable();
   }
   const RealVectorPtr yData = pcmData.copyToVectorData();
   RealVector xData( yData->size() );
   for ( size_t i = 0 ; i < yData->size(); ++ i )
   {
      xData[ i ] = i;
   }

   Plotting::RegLargeDataCurve* item = new Plotting::RegLargeDataCurve( xData, *yData );
   item->setLineColor( colour );
   item->setAntialiasing( true );
   item->setLineWidth( 2 );
   m_currentPlot->addItem( item );
   m_plotItems.push_back( item );
}

void QtPlotFactory::createScatter( const std::vector< double >& xData,
                                   const std::vector< double >& yData,
                                   const Plotting::MarkerDrawAttr& markerDrawAttr )
{
   if ( !m_currentPlot )
   {
      throw ExceptionNoPlotAvailable();
   }

   Plotting::ScatterItem * item = new Plotting::ScatterItem( xData, yData, markerDrawAttr );
   m_currentPlot->addItem( item );
   m_plotItems.push_back( item );
}

void QtPlotFactory::createStftGraph( const WaveAnalysis::StftData& stftData )
{
   if ( !m_currentPlot )
   {
      throw ExceptionNoPlotAvailable();
   }

   Plotting::StftPaintItem* item = new Plotting::StftPaintItem( stftData );
   m_currentPlot->addItem( item );
   m_plotItems.push_back( item );
}

void QtPlotFactory::createHistogram( const Math::IAccumArray& hist, const QColor& colour )
{
   if ( !m_currentPlot )
   {
      throw ExceptionNoPlotAvailable();
   }

   Plotting::HistogramItem* item = new Plotting::HistogramItem( hist );
   item->setLineColor( colour );
   item->setAntialiasing( true );
   m_currentPlot->addItem( item );
   m_plotItems.push_back( item );
}

void QtPlotFactory::create2DHist( const Math::Regular2DHistogram& hist, const Plotting::Palette& palette )
{
   if ( !m_currentPlot )
   {
      throw ExceptionNoPlotAvailable();
   }

   Plotting::Hist2DItem* item = new Plotting::Hist2DItem( hist, palette );
   m_currentPlot->addItem( item );
   m_currentPlot->setEnableGrid( false );
   m_currentPlot->setViewportConstraintsToData();
   m_plotItems.push_back( item );
}

void QtPlotFactory::createZScatter( const std::vector< double >& xData,
                                    const std::vector< double >& yData,
                                    const std::vector< double >& zData,
                                    const Plotting::Palette& palette,
                                    const Plotting::MarkerDrawAttr& markerDrawAttr )
{
   if ( !m_currentPlot )
   {
      throw ExceptionNoPlotAvailable();
   }

   Plotting::ZScatterItem * item = new Plotting::ZScatterItem( xData, yData, zData, palette, markerDrawAttr );
   m_currentPlot->addItem( item );
   m_plotItems.push_back( item );
}

} /// namespace PlotInterface
