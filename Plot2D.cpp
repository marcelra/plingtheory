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
#include <QDebug>

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
   isConnected = connect( m_graph, SIGNAL( viewportChanged( QRectF ) ), this, SLOT( synchroniseViewports( QRectF ) ) );
   assert( isConnected );
   isConnected = connect( m_xScroll, SIGNAL( viewportFromScroll( QRectF ) ), this, SLOT( setViewport( QRectF ) ) );
   assert( isConnected );
   isConnected = connect( m_yScroll, SIGNAL( viewportFromScroll( QRectF ) ), this, SLOT( setViewport( QRectF ) ) );
   assert( isConnected );

   /// Enable grid by default.
   setEnableGrid( true );
   synchroniseViewports( m_graph->getViewport() );
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
      m_graph->setGridItem( m_gridItem );
      m_gridItem->setLineColor( QColor( 220, 220, 220 ) );
      m_gridItem->setLineWidth( 1 );
      update();
   }
   else if ( !enableGrid )
   {
      if ( m_gridItem )
      {
         m_graph->setGridItem( 0 );
         delete m_gridItem;
         m_gridItem = 0;
         update();
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setEnableScrollOutsideDataRange
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Plot2D::setViewportConstraintsToData()
{
   m_graph->setViewportConstraints( m_graph->getDataRange() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// addItem
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Plot2D::addItem( const IPaintItem* item )
{
   m_graph->addPaintItem( item );
   m_graph->autoScale();

   const QRectF& dataRange = m_graph->getDataRange();

   m_xScroll->setDataRange( dataRange.left(), dataRange.right() );
   m_yScroll->setDataRange( dataRange.bottom(), dataRange.top() );
   m_xScroll->setViewport( m_graph->getViewport() );
   m_yScroll->setViewport( m_graph->getViewport() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setViewport
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Plot2D::setViewport( const QRectF& viewport )
{
  m_graph->setViewport( viewport );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// synchroniseViewports
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Plot2D::synchroniseViewports( const QRectF& viewport )
{
   m_xScroll->setViewport( viewport );
   m_yScroll->setViewport( viewport );
   m_xAxisPaintArea->setViewport( viewport );
   m_yAxisPaintArea->setViewport( viewport );
}

} /// namespace Plotting
