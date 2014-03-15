#include "VerticalScrollPaintArea.h"

namespace Plotting
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VerticalScrollPaintArea::VerticalScrollPaintArea( QWidget* parent ) :
   ScrollPaintArea( parent )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getDataRangeRect
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QRectF VerticalScrollPaintArea::getDataRangeRect() const
{
   QPointF xMinData( 0, m_dataMin );
   QPointF xMaxData( 0, m_dataMax );

   QPointF dataMinCanvas = transformToCanvasCoordinates( xMinData );
   QPointF dataMaxCanvas = transformToCanvasCoordinates( xMaxData );
   dataMinCanvas.setX( 0 );
   dataMaxCanvas.setX( m_canvas.right() + 1 );
   QRectF dataRect( dataMinCanvas, dataMaxCanvas );

   return dataRect;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getViewRangeRect
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QRectF VerticalScrollPaintArea::getViewRangeRect() const
{
   QPointF viewMinCanvas = transformToCanvasCoordinates( m_viewportGraph.topLeft() );
   QPointF viewMaxCanvas = transformToCanvasCoordinates( m_viewportGraph.bottomRight() );

   QRect viewRect( m_canvas );
   viewRect.setTop( viewMinCanvas.y() );
   viewRect.setBottom( viewMaxCanvas.y() );
   viewRect.setLeft( viewRect.left() + 1 );
   viewRect.setRight( viewRect.right() );

   /// Correct boundaries.
   if ( viewRect.top() == 0 )
   {
      viewRect.setTop( 1 );
   }
   if ( viewRect.bottom() > m_canvas.bottom() )
   {
      viewRect.setBottom( m_canvas.bottom() );
   }

   return viewRect;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// viewportChangedSlot
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VerticalScrollPaintArea::viewportChangedSlot( const QRectF& newViewportOfGraph )
{
   m_viewportGraph = newViewportOfGraph;

   /// Do not zoom out more than data range.
   if ( m_viewportGraph.top() < m_dataMax )
   {
      m_viewport.setTop( m_dataMax );
   }
   if ( m_viewportGraph.bottom() > m_dataMin )
   {
      m_viewport.setBottom( m_dataMin );
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// updateViewPortGraphFromShift
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VerticalScrollPaintArea::updateViewportGraphFromShift( const QPointF& shift )
{
   double oldHeight = -m_viewportGraph.height();

   m_viewportGraph.setTop( m_viewportGraph.top() + shift.y() );
   m_viewportGraph.setBottom( m_viewportGraph.bottom() + shift.y() );

   if ( m_viewportGraph.top() > m_viewport.top() )
   {
      m_viewportGraph.setTop( m_viewport.top() );
      m_viewportGraph.setBottom( m_viewport.top() - oldHeight );
   }
   if ( m_viewportGraph.bottom() < m_viewport.bottom() )
   {
      m_viewportGraph.setTop( m_viewport.bottom() + oldHeight );
      m_viewportGraph.setBottom( m_viewport.bottom() );
   }
}

} /// namespace Plotting
