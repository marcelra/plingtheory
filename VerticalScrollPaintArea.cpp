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
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VerticalScrollPaintArea::~VerticalScrollPaintArea()
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
   QPointF viewMinCanvas = transformToCanvasCoordinates( m_viewPortGraph.topLeft() );
   QPointF viewMaxCanvas = transformToCanvasCoordinates( m_viewPortGraph.bottomRight() );

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
/// viewPortChangedSlot
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VerticalScrollPaintArea::viewPortChangedSlot( const QRectF& newViewPort )
{
   m_viewPortGraph = newViewPort;

   /// Do not zoom out more than data range.
   if ( m_viewPortGraph.top() < m_dataMax )
   {
      m_viewPort.setTop( m_dataMax );
   }
   if ( m_viewPortGraph.bottom() > m_dataMin )
   {
      m_viewPort.setBottom( m_dataMin );
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// updateViewPortGraphFromShift
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VerticalScrollPaintArea::updateViewPortGraphFromShift( const QPointF& shift )
{
   double oldHeight = -m_viewPortGraph.height();

   m_viewPortGraph.setTop( m_viewPortGraph.top() + shift.y() );
   m_viewPortGraph.setBottom( m_viewPortGraph.bottom() + shift.y() );

   if ( m_viewPortGraph.top() > m_viewPort.top() )
   {
      m_viewPortGraph.setTop( m_viewPort.top() );
      m_viewPortGraph.setBottom( m_viewPort.top() - oldHeight );
   }
   if ( m_viewPortGraph.bottom() < m_viewPort.bottom() )
   {
      m_viewPortGraph.setTop( m_viewPort.bottom() + oldHeight );
      m_viewPortGraph.setBottom( m_viewPort.bottom() );
   }
}

} /// namespace Plotting
