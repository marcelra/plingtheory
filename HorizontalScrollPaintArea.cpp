#include "HorizontalScrollPaintArea.h"

namespace Plotting
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HorizontalScrollPaintArea::HorizontalScrollPaintArea( QWidget* parent ) :
   ScrollPaintArea( parent )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getDataRangeRect
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QRectF HorizontalScrollPaintArea::getDataRangeRect() const
{
   QPointF xMinData( m_dataMin, 0 );
   QPointF xMaxData( m_dataMax, 0 );

   QPointF dataMinCanvas = transformToCanvasCoordinates( xMinData );
   QPointF dataMaxCanvas = transformToCanvasCoordinates( xMaxData );
   dataMinCanvas.setY( 0 );
   dataMaxCanvas.setY( m_canvas.bottom() + 1 );
   QRectF dataRect( dataMinCanvas, dataMaxCanvas );

   return dataRect;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getViewRangeRect
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QRectF HorizontalScrollPaintArea::getViewRangeRect() const
{
   QPointF viewMinCanvas = transformToCanvasCoordinates( m_viewportGraph.topLeft() );
   QPointF viewMaxCanvas = transformToCanvasCoordinates( m_viewportGraph.bottomRight() );

   QRectF viewRect( m_canvas );
   viewRect.setLeft( viewMinCanvas.x() );
   viewRect.setRight( viewMaxCanvas.x() );
   viewRect.setTop( viewRect.top() + 1 );
   viewRect.setBottom( viewRect.bottom() );
   if ( viewRect.left() == 0 )
   {
      viewRect.setLeft( 1 );
   }
   if ( viewRect.right() == m_canvas.right() )
   {
      viewRect.setRight( m_canvas.right() - 1 );
   }

   return viewRect;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// viewportChangedSlot
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HorizontalScrollPaintArea::viewportChangedSlot( const QRectF& newViewportOfGraph )
{
   m_viewportGraph = newViewportOfGraph;
   if ( m_viewportGraph.left() > m_dataMin )
   {
      m_viewport.setLeft( m_dataMin );
   }
   if ( m_viewportGraph.right() < m_dataMax )
   {
      m_viewport.setRight( m_dataMax );
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// updateViewPortGraphFromShift
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HorizontalScrollPaintArea::updateViewportGraphFromShift( const QPointF& shift )
{
   double oldWidth = m_viewportGraph.width();
   m_viewportGraph.setLeft( m_viewportGraph.left() + shift.x() );
   m_viewportGraph.setRight( m_viewportGraph.right() + shift.x() );

   if ( m_viewportGraph.left() < m_viewport.left() )
   {
      m_viewportGraph.setLeft( m_viewport.left() );
      m_viewportGraph.setRight( m_viewport.left() + oldWidth );
   }
   if ( m_viewportGraph.right() > m_viewport.right() )
   {
      m_viewportGraph.setLeft( m_viewport.right() - oldWidth );
      m_viewportGraph.setRight( m_viewport.right() );
   }
}

} /// namespace Plotting
