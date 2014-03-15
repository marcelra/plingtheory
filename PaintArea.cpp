#include "PaintArea.h"

#include "IPaintCommand.h"
#include "IPaintItem.h"

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
   m_paintItems(),
   m_paintCommands(),
   m_horizontalMouseWheel( true )
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

   /// Regenerate paint commands if needed
   generatePaintCommands();

   /// Execute all paint commands.
   for ( size_t i = 0; i < m_paintCommands.size(); ++i )
   {
      m_paintCommands[ i ]->execute( *this );
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

} /// namespace Plotting
