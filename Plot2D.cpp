#include "Plot2D.h"

/// Plotting includes
#include "CurveItem.h"
#include "GridItem.h"
#include "HorizontalScrollPaintArea.h"
#include "PaintArea.h"
#include "VerticalScrollPaintArea.h"
#include "XAxisPaintArea.h"
#include "YAxisPaintArea.h"

/// Qt includes
#include <QGridLayout>

/// STL includes
#include <cassert>

namespace Plotting
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Plot2D::Plot2D( QWidget* parent ) :
   QWidget( parent ),
   m_gridItem( 0 )
{
   /// Construct widgets.
   m_xAxisPaintArea = new XAxisPaintArea( this );
   m_yAxisPaintArea = new YAxisPaintArea( this );
   m_graph = new PaintArea( this );
   m_empty = new PaintArea( this );
   m_xScroll = new HorizontalScrollPaintArea( this );
   m_yScroll = new VerticalScrollPaintArea( this );

   /// Set Layout.
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
   setLayout( m_gridLayout );

   /// Set debug names.
   m_graph->setName( "graph" );
   m_empty->setName( "empty" );
   m_xScroll->setName( "xScroll" );
   m_yScroll->setName( "yScroll" );
   m_xAxisPaintArea->setName( "xAxis" );
   m_yAxisPaintArea->setName( "yAxis" );

   /// Make connections.
   bool isConnected;
   isConnected = connect( m_graph, SIGNAL( viewPortChanged( QRectF ) ), this, SLOT( synchroniseViewPorts( QRectF ) ) );
   assert( isConnected );
   isConnected = connect( m_xScroll, SIGNAL( viewPortFromScroll( QRectF ) ), this, SLOT( setViewPort( QRectF ) ) );
   assert( isConnected );
   isConnected = connect( m_yScroll, SIGNAL( viewPortFromScroll( QRectF ) ), this, SLOT( setViewPort( QRectF ) ) );
   assert( isConnected );

   /// Enable grid by default.
   setEnableGrid( true );
   synchroniseViewPorts( m_graph->getViewport() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Plot2D::~Plot2D()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setEnableGrid
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// addItem
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Plot2D::addItem( const IPaintItem* item )
{
   item->paintOn( m_graph );

   m_graph->autoScale();
   const QRectF& dataRange = m_graph->getDataRange();

   m_xScroll->setDataRange( dataRange.left(), dataRange.right() );
   m_yScroll->setDataRange( dataRange.bottom(), dataRange.top() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setViewPort
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Plot2D::setViewPort( const QRectF& viewPort )
{
  m_graph->setViewport( viewPort );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// synchroniseViewPorts
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Plot2D::synchroniseViewPorts( const QRectF& viewPort )
{
   m_xScroll->setViewport( viewPort );
   m_yScroll->setViewport( viewPort );
   m_xAxisPaintArea->setViewport( viewPort );
   m_yAxisPaintArea->setViewport( viewPort );
}

} /// namespace Plotting
