#include "PaintArea.h"

#include "IPaintCommand.h"
#include "IPaintItem.h"

#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QRectF>

#include <cassert>
#include <limits>

namespace Plotting
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PaintArea::PaintArea( QWidget* parent ) :
   PaintAreaBase( parent ),
   m_paintItems(),
   m_paintCommands(),
   m_horizontalMouseWheel( true ),
   m_oldMousePos( 0 )
{
   m_dataRange = QRectF( QPointF( 0, 0 ), QPointF( 0, 0 ) );
   QRectF initialViewPort( QPointF( -1, -1 ), QPointF( 1, 1 ) );
   setViewPort( initialViewPort );
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

   // qDebug() << "In paintEventImpl for " << m_name;

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
   // qDebug() << "m_name = " << m_name;
   m_paintItems.push_back( paintItem );

   if ( m_paintItems.size() == 1 )
   {
      m_dataRange.setLeft( paintItem->getMinX() );
      m_dataRange.setRight( paintItem->getMaxX() );
      m_dataRange.setBottom( paintItem->getMinY() );
      m_dataRange.setTop( paintItem->getMaxY() );
   }
   else
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
   // qDebug() << m_name << " AUTOSCALE: " << m_dataRange;
   m_viewPort = m_dataRange;
   emit viewPortChanged( m_viewPort );
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
   const QPointF& viewPortCenter = transformToWorldCoordinates( event->pos() );
   const QRectF& viewPort = getViewPort();

   if ( m_horizontalMouseWheel )
   {
      double newWidth = viewPort.width();
      newWidth *= event->delta() > 0 ? 0.75 : 1.25;

      QRectF newViewPort( viewPort );

      double x = viewPortCenter.x() - viewPort.left();
      double viewPortShift = x - x / viewPort.width() * newWidth;

      newViewPort.setLeft( viewPort.left() + viewPortShift );
      newViewPort.setWidth( newWidth );

      setViewPort( newViewPort );
      update();
   }
   else
   {
      double newHeight = viewPort.height();
      newHeight *= event->delta() > 0 ? 0.75 : 1.25;

      QRectF newViewPort( viewPort );

      double y = viewPortCenter.y();
      double newBottom = newHeight / viewPort.height() * ( y + viewPort.bottom() ) - y;

      newViewPort.setTop( newBottom - newHeight );
      newViewPort.setHeight( newHeight );

      setViewPort( newViewPort );
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
   // qDebug() << "Mouse move: " << m_name;
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
   QRectF viewPort = getViewPort();
   viewPort.setLeft( viewPort.left() - shiftOfViewport.x() );
   viewPort.setRight( viewPort.right() - shiftOfViewport.x() );
   viewPort.setTop( viewPort.top() + shiftOfViewport.y() );
   viewPort.setBottom( viewPort.bottom() + shiftOfViewport.y() );

   /// Apply new viewport.
   setViewPort( viewPort );

   /// Update mouse position.
   *m_oldMousePos = event->pos();

   /// Set event handled.
   event->accept();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// mouseReleaseEvent
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintArea::mouseReleaseEvent( QMouseEvent* event )
{
   if ( m_oldMousePos )
   {
      delete m_oldMousePos;
      m_oldMousePos = 0;
   }
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
