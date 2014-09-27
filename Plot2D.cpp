#include "Plot2D.h"

/// Plotting includes
#include "CurveItem.h"
#include "GridItem.h"
#include "HorizontalScrollPaintArea.h"
#include "VerticalScrollPaintArea.h"
#include "XAxisPaintArea.h"
#include "YAxisPaintArea.h"

/// Qt includes
#include <QDebug>
#include <QFileDialog>
#include <QGridLayout>
#include <QMessageBox>

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
   m_xScroll->setFixedHeight( 21 );
   m_yScroll->setFixedWidth( 21 );

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
   isConnected = connect( m_xScroll, SIGNAL( updateViewportFromMarkers() ), this, SLOT( synchroniseWithGraphViewport() ) );
   assert( isConnected );
   isConnected = connect( m_yScroll, SIGNAL( updateViewportFromMarkers() ), this, SLOT( synchroniseWithGraphViewport() ) );
   assert( isConnected );
   isConnected = connect( m_graph, SIGNAL( zoomModeChanged( PaintArea::ZoomMode ) ), this, SLOT( animateActiveScrollbar( PaintArea::ZoomMode ) ) );
   assert( isConnected );
   isConnected = connect( m_graph, SIGNAL( exportPlot() ), this, SLOT( exportPlot() ) );
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// synchroniseWithGraphViewport
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Plot2D::synchroniseWithGraphViewport()
{
   m_xScroll->setViewport( m_graph->getViewport() );
   m_yScroll->setViewport( m_graph->getViewport() );
   m_xAxisPaintArea->setViewport( m_graph->getViewport() );
   m_yAxisPaintArea->setViewport( m_graph->getViewport() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// animateActiveScrollbar
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Plot2D::animateActiveScrollbar( PaintArea::ZoomMode zoomMode )
{
   if ( zoomMode == PaintArea::ZoomHorizontal )
   {
      m_xScroll->animateActive();
   }
   else if ( zoomMode == PaintArea::ZoomVertical )
   {
      m_yScroll->animateActive();
   }
   else if ( zoomMode == PaintArea::ZoomBoth )
   {
      m_xScroll->animateActive();
      m_yScroll->animateActive();
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// exportPlot
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Plot2D::exportPlot()
{
   QString fileName = QFileDialog::getSaveFileName( this, tr( "Export plot" ), ".", tr( "Images (*.png *.xpm *.jpg)" ) );

   QPalette::ColorRole prevBkgRole = backgroundRole();
   setBackgroundRole( QPalette::Base );
   setAutoFillBackground( true );

   m_xScroll->setVisible( false );
   m_yScroll->setVisible( false );

   QPixmap exportedPlot( size() );
   render( &exportedPlot );

   m_xScroll->setVisible( true );
   m_yScroll->setVisible( true );

   setBackgroundRole( prevBkgRole );
   setAutoFillBackground( true );

   bool success = exportedPlot.save( fileName );

   if ( success )
   {
      QString msg( "Plot saved as ");
      msg += fileName;
      QMessageBox::information( this, "Export complete", msg );
   }
   else
   {
      QString msg( "Could not export plot. Check write permissions for the location specified." );
      QMessageBox::warning( this, "Export failed", msg );
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setXAxisTitle
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Plot2D::setXAxisTitle( const std::string& xAxisTitle )
{
   m_xAxisPaintArea->setTitle( xAxisTitle );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setYAxisTitle
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Plot2D::setYAxisTitle( const std::string& yAxisTitle )
{
   m_yAxisPaintArea->setTitle( yAxisTitle );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getXAxisTitle
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const std::string& Plot2D::getXAxisTitle() const
{
   return m_xAxisPaintArea->getTitle();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getYAxisTitle
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const std::string& Plot2D::getYAxisTitle() const
{
   return m_yAxisPaintArea->getTitle();
}

} /// namespace Plotting
