#include "Plot2D.h"

#include "CurveItem.h"
#include "GridItem.h"
#include "HorizontalScrollPaintArea.h"
#include "PaintArea.h"
#include "VerticalScrollPaintArea.h"
#include "XAxisPaintArea.h"
#include "YAxisPaintArea.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QWheelEvent>

#include <cassert>
#include <limits>

namespace Plotting
{

Plot2D::Plot2D( QWidget* parent ) :
   QWidget( parent ),
   m_gridItem( 0 )
{
   m_xAxisPaintArea = new XAxisPaintArea( this );
   m_xAxisPaintArea->setName( "xAxis" );
   m_yAxisPaintArea = new YAxisPaintArea( this );
   m_yAxisPaintArea->setName( "yAxis" );
   m_graph = new PaintArea( this );
   m_empty = new PaintArea( this );
   m_xScroll = new HorizontalScrollPaintArea( this );
   m_xScroll->setName( "xScroll" );
   m_yScroll = new VerticalScrollPaintArea( this );
   m_yScroll->setName( "yScroll" );

   m_xScroll->setFixedHeight( 20 );
   m_yScroll->setFixedWidth( 20 );

   m_empty->setFixedHeight( 100 );
   m_empty->setFixedWidth( 100 );

   m_gridLayout = new QGridLayout( this );
   m_gridLayout->addWidget( m_yAxisPaintArea, 1, 0 );
   m_gridLayout->addWidget( m_empty, 2, 0 );

   m_gridLayout->addWidget( m_xScroll, 0, 1 );
   m_gridLayout->addWidget( m_graph, 1, 1 );
   m_gridLayout->addWidget( m_xAxisPaintArea, 2, 1 );

   m_gridLayout->addWidget( m_yScroll, 1, 2 );

   m_gridLayout->setSpacing( 5 );
   m_gridLayout->setContentsMargins( 0, 0, 0, 0 );

   m_graph->setName( "graph" );
   m_empty->setName( "empty" );

   std::vector< double> xAxisPointsX;
   xAxisPointsX.push_back( -std::numeric_limits< double >::max() );
   xAxisPointsX.push_back( std::numeric_limits< double >::max() );
   std::vector< double> xAxisPointsY;
   xAxisPointsY.push_back( 0 );
   xAxisPointsY.push_back( 0 );
   // m_xAxis = new CurveItem( xAxisPointsX, xAxisPointsY );
   // addCurve( m_xAxis );

   std::vector< double> yAxisPointsX;
   yAxisPointsX.push_back( 0 );
   yAxisPointsX.push_back( 0 );
   std::vector< double> yAxisPointsY;
   yAxisPointsY.push_back( -std::numeric_limits< double >::max() );
   yAxisPointsY.push_back( std::numeric_limits< double >::max() );
   // m_yAxis = new CurveItem( yAxisPointsX, yAxisPointsY );
   // addCurve( m_yAxis );


   setLayout( m_gridLayout );

   bool isConnected;
   isConnected = connect( m_graph, SIGNAL( viewPortChanged( QRectF ) ), this, SLOT( synchroniseViewPorts( QRectF ) ) );
   assert( isConnected );
   isConnected = connect( m_xScroll, SIGNAL( viewPortFromScroll( QRectF ) ), this, SLOT( setViewPort( QRectF ) ) );
   assert( isConnected );
   isConnected = connect( m_yScroll, SIGNAL( viewPortFromScroll( QRectF ) ), this, SLOT( setViewPort( QRectF ) ) );
   assert( isConnected );

   setEnableGrid( true );

   // setLayout( m_horizontalLayout );
}

Plot2D::~Plot2D()
{
}

void Plot2D::setEnableGrid( bool enableGrid )
{
   if ( enableGrid && !m_gridItem )
   {
      m_gridItem = new GridItem( *m_xAxisPaintArea, *m_yAxisPaintArea );
      m_graph->addPaintItem( m_gridItem );
      m_gridItem->setLineColor( QColor( 220, 220, 220 ) );
      m_gridItem->setLineWidth( 1 );
      update();
   }
   else if ( !enableGrid )
   {
      if ( m_gridItem )
      {
         m_graph->removePaintItem( m_gridItem );
         delete m_gridItem;
         m_gridItem = 0;
         update();
      }
   }
}

void Plot2D::addItem( const IPaintItem* item )
{
   item->paintOn( m_graph );

   m_graph->autoScale();
   const QRectF& dataRange = m_graph->getDataRange();

   m_xScroll->setDataRange( dataRange.left(), dataRange.right() );
   m_yScroll->setDataRange( dataRange.bottom(), dataRange.top() );
}

void Plot2D::setViewPort( const QRectF& viewPort )
{
  m_graph->setViewPort( viewPort );
}

void Plot2D::synchroniseViewPorts( const QRectF& viewPort )
{
   m_xScroll->setViewPort( viewPort );
   m_yScroll->setViewPort( viewPort );
   m_xAxisPaintArea->setViewPort( viewPort );
   m_yAxisPaintArea->setViewPort( viewPort );
}


}
