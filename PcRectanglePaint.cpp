#include "PcRectanglePaint.h"

#include "PaintArea.h"

#include <QPainter>

namespace Plotting
{

PcRectanglePaint::PcRectanglePaint( const QPointF& topLeft, const QPointF& bottomRight ) :
   m_rect( topLeft, bottomRight )
{}

PcRectanglePaint::~PcRectanglePaint()
{}

void PcRectanglePaint::execute( PaintArea& paintArea ) const
{
   const QPointF& topLeft_transformed = paintArea.transformToCanvasCoordinates( m_rect.topLeft() );
   const QPointF& bottomRight_transformed = paintArea.transformToCanvasCoordinates( m_rect.bottomRight() );
   QRectF rect_transformed( topLeft_transformed, bottomRight_transformed );
   paintArea.getPainter().drawRect( rect_transformed );
}

} /// namespace Plotting
