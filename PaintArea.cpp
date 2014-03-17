#include "PaintArea.h"

#include "IPaintCommand.h"
#include "IPaintItem.h"
#include "GridItem.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QRectF>

#include <cassert>
#include <limits>

#include <QDebug>

namespace Plotting
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PaintArea::PaintArea( QWidget* parent ) :
   PaintAreaBase( parent ),
   m_gridItem( 0 ),
   m_horizontalMouseWheel( true ),
   m_zoomAreaStart( 0 )
{
   m_dataRange = QRectF( QPointF( -1, 1 ), QPointF( 1, -1 ) );
   QRectF initialViewPort( QPointF( -1, 1 ), QPointF( 1, -1 ) );
   setViewport( initialViewPort );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PaintArea::~PaintArea()
{
   clearPaintCommands();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// clearPaintCommands
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintArea::clearPaintCommands()
{
   for ( size_t i = 0; i < m_paintCommands.size(); ++ i )
   {
      delete m_paintCommands[ i ];
   }
   m_paintCommands.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// generatePaintCommands
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintArea::generatePaintCommands()
{
   /// Clear all existing paint commands.
   clearPaintCommands();

   /// Generate commands from grid item first.
   if ( m_gridItem )
   {
      m_gridItem->generatePlotCommands( this );
   }

   /// Let the paint items generate paint commands.
   for ( size_t iPaintItem = 0; iPaintItem < m_paintItems.size(); ++iPaintItem )
   {
      m_paintItems[ iPaintItem ]->generatePlotCommands( this );
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// paintEvent
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintArea::paintEventImpl( QPaintEvent* event )
{
   assert( event );

   if ( !m_zoomAreaStart.get() )
   {
      /// Regenerate paint commands if needed
      generatePaintCommands();

      /// Execute all paint commands.
      for ( size_t i = 0; i < m_paintCommands.size(); ++i )
      {
         m_paintCommands[ i ]->execute( *this );
      }
   }
   else
   {
      getPainter().drawPixmap( 0, 0, m_grabbedWidget );

      getPainter().save();
      if ( m_zoomAreaEnd.get() )
      {
         QColor color;
         QBrush brush;

         QRect zoomArea;
         zoomArea.setLeft( std::min( m_zoomAreaStart->x(), m_zoomAreaEnd->x() ) );
         zoomArea.setRight( std::max( m_zoomAreaStart->x(), m_zoomAreaEnd->x() ) );
         zoomArea.setBottom( std::max( m_zoomAreaStart->y(), m_zoomAreaEnd->y() ) );
         zoomArea.setTop( std::min( m_zoomAreaStart->y(), m_zoomAreaEnd->y() ) );

         QRect leftRect( m_canvas.topLeft(), QPoint( zoomArea.left(), m_canvas.bottom() ) );
         QRect rightRect( QPoint( zoomArea.right(), m_canvas.top() ), m_canvas.bottomRight() );
         QRect topRect( QPoint( zoomArea.left() + 1, m_canvas.top() ), QPoint( zoomArea.right() - 1, zoomArea.top() ) );
         QRect bottomRect( QPoint( zoomArea.left() + 1, zoomArea.bottom() ), QPoint( zoomArea.right() - 1, m_canvas.bottom() ) );

         color = QColor( Qt::lightGray );
         color.setAlpha( 128 );
         brush = QBrush( color, Qt::SolidPattern );
         getPainter().setBrush( brush );
         getPainter().setPen( QPen( Qt::NoPen ) );

         getPainter().drawRect( leftRect );
         getPainter().drawRect( rightRect );
         getPainter().drawRect( topRect );
         getPainter().drawRect( bottomRect );

         getPainter().setBrush( QBrush( Qt::NoBrush ) );
         getPainter().setPen( QPen( color, 2 ) );
         getPainter().drawRect( zoomArea );
      }
      getPainter().restore();
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// addPaintItem
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintArea::addPaintItem( const IPaintItem* paintItem )
{
   m_paintItems.push_back( paintItem );

   if ( m_paintItems.size() == 1 )
   {
      m_dataRange.setLeft( paintItem->getMinX() );
      m_dataRange.setRight( paintItem->getMaxX() );
      m_dataRange.setBottom( paintItem->getMinY() );
      m_dataRange.setTop( paintItem->getMaxY() );
   }
   else /// TODO: for robustness: loop over all paint items and determine min/max.
   {
      if ( paintItem->getMinX() < m_dataRange.left() )
      {
         m_dataRange.setLeft( paintItem->getMinX() );
      }
      if ( paintItem->getMaxX() > m_dataRange.right() )
      {
         m_dataRange.setRight( paintItem->getMaxX() );
      }
      if ( paintItem->getMinY() < m_dataRange.bottom() )
      {
         m_dataRange.setBottom( paintItem->getMinY() );
      }
      if ( paintItem->getMaxY() > m_dataRange.top() )
      {
         m_dataRange.setTop( paintItem->getMaxY() );
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// autoScale
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintArea::autoScale()
{
   double yBorder = -0.1 * m_dataRange.height();
   double xBorder = 0.1 * m_dataRange.width();

   m_viewport.setTop( m_dataRange.top() + yBorder );
   m_viewport.setBottom( m_dataRange.bottom() - yBorder );
   m_viewport.setLeft( m_dataRange.left() - xBorder );
   m_viewport.setRight( m_dataRange.right() + xBorder );

   emit viewportChanged( m_viewport );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getDataRange
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const QRectF& PaintArea::getDataRange() const
{
   return m_dataRange;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// addPaintCommand
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintArea::addPaintCommand( const IPaintCommand* paintCommand )
{
   m_paintCommands.push_back( paintCommand );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// wheelEvent
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintArea::wheelEvent( QWheelEvent* event )
{
   const QPointF& viewportCenter = transformToWorldCoordinates( event->pos() );
   const QRectF& viewport = getViewport();

   if ( m_horizontalMouseWheel )
   {
      double newWidth = viewport.width();
      newWidth *= event->delta() > 0 ? 0.75 : 1.25;

      QRectF newViewPort( viewport );

      double x = viewportCenter.x() - viewport.left();
      double viewportShift = x - x / viewport.width() * newWidth;

      newViewPort.setLeft( viewport.left() + viewportShift );
      newViewPort.setWidth( newWidth );

      setViewport( newViewPort );
      update();
   }
   else
   {
      double newHeight = viewport.height();
      newHeight *= event->delta() > 0 ? 0.75 : 1.25;

      QRectF newViewPort( viewport );

      double y = viewportCenter.y();
      double newTop = newHeight/viewport.height() * newViewPort.top() + y * ( 1.0 - newHeight/viewport.height() );

      newViewPort.setTop( newTop );
      newViewPort.setBottom( newTop + newHeight );

      setViewport( newViewPort );
      update();
   }

   event->accept();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// mouseDoubleClickEvent
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintArea::mouseDoubleClickEvent( QMouseEvent* event )
{
   m_horizontalMouseWheel = !m_horizontalMouseWheel;
   event->accept();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// mouseMoveEvent
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintArea::mouseMoveEvent( QMouseEvent* event )
{
   /// Zooming with right button.
   if ( m_zoomAreaStart.get() )
   {
      m_zoomAreaEnd.reset( new QPoint( event->pos() ) );
      event->accept();
      update();
   }
   /// Panning with left button.
   else
   {
      /// Store old mouse pos if it does not exist.
      if ( !m_oldMousePos )
      {
         m_oldMousePos = new QPoint( event->pos() );
         event->accept();
         return;
      }

      /// Calculate shift in world coordinates.
      const QPointF& shiftOfViewport = transformToWorldCoordinates( event->pos() ) - transformToWorldCoordinates( *m_oldMousePos );

      /// Modify current viewport.
      QRectF viewport = getViewport();
      viewport.setLeft( viewport.left() - shiftOfViewport.x() );
      viewport.setRight( viewport.right() - shiftOfViewport.x() );
      viewport.setTop( viewport.top() - shiftOfViewport.y() );
      viewport.setBottom( viewport.bottom() - shiftOfViewport.y() );

      /// Apply new viewport.
      setViewport( viewport );

      /// Update mouse position.
      *m_oldMousePos = event->pos();

      /// Set event handled.
      event->accept();
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// clearAllPaintItems
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintArea::clearAllPaintItems()
{
   m_paintItems.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// removePaintItem
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintArea::removePaintItem( const IPaintItem* paintItem )
{
   for ( int iPaintItem = m_paintItems.size() - 1; iPaintItem >= 0; --iPaintItem )
   {
      if ( m_paintItems[ iPaintItem ] == paintItem )
      {
         m_paintItems.erase( m_paintItems.begin() + iPaintItem );
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// addGridItem
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintArea::setGridItem( const GridItem* gridItem )
{
   m_gridItem = gridItem;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// mouseReleaseEvent
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 void PaintArea::mouseReleaseEvent( QMouseEvent* event )
{
   if ( event->button() == Qt::RightButton && m_zoomAreaStart.get() )
   {
      const QPointF& p0 = transformToWorldCoordinates( *m_zoomAreaStart );
      const QPointF& p1 = transformToWorldCoordinates( *m_zoomAreaEnd );

      QRectF newViewport;
      newViewport.setLeft( std::min( p0.x(), p1.x() ) );
      newViewport.setRight( std::max( p0.x(), p1.x() ) );
      newViewport.setBottom( std::min( p0.y(), p1.y() ) );
      newViewport.setTop( std::max( p0.y(), p1.y() ) );

      setViewport( newViewport );

      m_zoomAreaStart.reset( 0 );
      m_zoomAreaEnd.reset( 0 );

      event->accept();
      update();
   }
   else
   {
      PaintAreaBase::mouseReleaseEvent( event );
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// mousePressEvent
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintArea::mousePressEvent( QMouseEvent* event )
{
   if ( event->button() == Qt::RightButton )
   {
      m_grabbedWidget = QPixmap( size() );
      render( &m_grabbedWidget );

      m_zoomAreaStart.reset( new QPoint( event->pos() ) );

      event->accept();
   }
   if ( event->button() == Qt::LeftButton && m_zoomAreaStart.get() )
   {
      m_zoomAreaStart.reset( 0 );
      m_zoomAreaEnd.reset( 0 );

      event->accept();

      update();
   }
}

} /// namespace Plotting
