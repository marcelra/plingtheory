#include "PcMarkerPaint.h"

#include "PaintArea.h"

#include <QPainter>

namespace Plotting
{

PcMarkerPaint::PcMarkerPaint( const QPointF& location, double markerSize ) :
   m_location( location ),
   m_markerSize( markerSize )
{}

PcMarkerPaint::~PcMarkerPaint()
{}

void PcMarkerPaint::execute( PaintArea& paintArea ) const
{
   QPainter& p = paintArea.getPainter();

   QPointF locationCanvas = paintArea.transformToCanvasCoordinates( m_location );

   QPointF topLeft = locationCanvas - 0.5 * m_markerSize * QPointF( 1, 1 );
   QPointF bottomRight = locationCanvas + 0.5 * m_markerSize * QPointF( 1, 1 );

   p.drawRect( QRectF( topLeft, bottomRight ) );
}

} /// namespace Plotting
