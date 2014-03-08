#include "HorizontalScrollPaintArea.h"

namespace Plotting
{

HorizontalScrollPaintArea::HorizontalScrollPaintArea( QWidget* parent ) :
   ScrollPaintArea( parent )
{}

HorizontalScrollPaintArea::~HorizontalScrollPaintArea()
{}

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

QRectF HorizontalScrollPaintArea::getViewRangeRect() const
{
   QPointF viewMinCanvas = transformToCanvasCoordinates( m_viewPortGraph.topLeft() );
   QPointF viewMaxCanvas = transformToCanvasCoordinates( m_viewPortGraph.bottomRight() );

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

void HorizontalScrollPaintArea::viewPortChangedSlot( const QRectF& newViewPort )
{
   m_viewPortGraph = newViewPort;
   if ( m_viewPortGraph.left() > m_dataMin )
   {
      m_viewPort.setLeft( m_dataMin );
   }
   if ( m_viewPortGraph.right() < m_dataMax )
   {
      m_viewPort.setRight( m_dataMax );
   }
}

} /// namespace Plotting
