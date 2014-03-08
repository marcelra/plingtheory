#include "VerticalScrollPaintArea.h"

#include <QDebug>

namespace Plotting
{

VerticalScrollPaintArea::VerticalScrollPaintArea( QWidget* parent ) :
   ScrollPaintArea( parent )
{}

VerticalScrollPaintArea::~VerticalScrollPaintArea()
{}

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

QRectF VerticalScrollPaintArea::getViewRangeRect() const
{
   QPointF viewMinCanvas = transformToCanvasCoordinates( m_viewPortGraph.bottomRight() );
   QPointF viewMaxCanvas = transformToCanvasCoordinates( m_viewPortGraph.topLeft() );

   QRect viewRect( m_canvas );
   viewRect.setTop( viewMinCanvas.y() );
   viewRect.setBottom( viewMaxCanvas.y() );
   viewRect.setLeft( viewRect.left() + 1 );
   viewRect.setRight( viewRect.right() );
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
   if ( m_viewPortGraph.bottom() < m_dataMax )
   {
      m_viewPort.setBottom( m_dataMax );
   }
   if ( m_viewPortGraph.top() > m_dataMin )
   {
      m_viewPort.setTop( m_dataMin );
   }
}

} /// namespace Plotting
