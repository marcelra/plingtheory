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
   viewRect.setRight( viewRect.right() - 1 );

   /// Correct boundaries.
   if ( viewRect.top() == 0 )
   {
      viewRect.setTop( 1 );
   }
   if ( viewRect.bottom() >= m_canvas.bottom() )
   {
      viewRect.setBottom( m_canvas.bottom() - 1 );
   }

   return viewRect;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// viewportChangedSlot
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VerticalScrollPaintArea::viewportGraphChanged( const QRectF& newViewportOfGraph )
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
/// updateViewportGraphFromShift
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// project
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double VerticalScrollPaintArea::project( const QPointF& p ) const
{
   return p.y();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getCanVecAlong
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QPointF VerticalScrollPaintArea::getCanVecAlong() const
{
   return QPointF( 0, m_canvas.height() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getCanVecAlongUnitVector
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QPointF VerticalScrollPaintArea::getCanVecAlongUnitVector() const
{
   return QPointF( 0, 1 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getCanVecOrthogonal
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QPointF VerticalScrollPaintArea::getCanVecOrthogonal() const
{
   return QPointF( m_canvas.width(), 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getCanVecOrthogonalUnitVector
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QPointF VerticalScrollPaintArea::getCanVecOrthogonalUnitVector() const
{
   return QPointF( 1, 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getBoundingValuesViewportGraph
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::pair< double, double > VerticalScrollPaintArea::getBoundingValuesViewportGraph() const
{
   return std::pair< double, double >( m_viewportGraph.bottom(), m_viewportGraph.top() );
}

} /// namespace Plotting
