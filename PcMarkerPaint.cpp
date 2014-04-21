#include "PcMarkerPaint.h"

#include "PaintArea.h"

#include <QPainter>

namespace Plotting
{

PcMarkerPaint::PcMarkerPaint( const QPointF& location, double markerSize, MarkerType markerType ) :
   m_location( location ),
   m_markerSize( markerSize ),
   m_markerType( markerType )
{}

PcMarkerPaint::~PcMarkerPaint()
{}

void PcMarkerPaint::execute( PaintArea& paintArea ) const
{
   QPainter& p = paintArea.getPainter();

   QPointF locationCanvas = paintArea.transformToCanvasCoordinates( m_location );

   if ( m_markerType == MarkerRectangle )
   {
      QPointF topLeft = locationCanvas - m_markerSize * QPointF( 1, 1 );
      QPointF bottomRight = locationCanvas + m_markerSize * QPointF( 1, 1 );

      p.drawRect( QRectF( topLeft, bottomRight ) );
   }
   else if ( m_markerType == MarkerCircle )
   {
      p.drawEllipse( locationCanvas, m_markerSize, m_markerSize );
   }
   else if ( m_markerType == MarkerCross )
   {
      QPointF topLeft = locationCanvas + m_markerSize * QPointF( -1, -1 );
      QPointF topRight = locationCanvas + m_markerSize * QPointF( 1, -1 );
      QPointF bottomLeft = locationCanvas + m_markerSize * QPointF( -1, 1 );
      QPointF bottomRight = locationCanvas + m_markerSize * QPointF( 1, 1 );

      p.drawLine( topLeft, bottomRight );
      p.drawLine( bottomLeft, topRight );
   }
   else if ( m_markerType == MarkerPlus )
   {
      QPointF top = locationCanvas + m_markerSize * QPointF( 0, -1 );
      QPointF bottom = locationCanvas + m_markerSize * QPointF( 0, 1 );
      QPointF left = locationCanvas + m_markerSize * QPointF( -1, 0 );
      QPointF right = locationCanvas + m_markerSize * QPointF( 1, 0 );

      p.drawLine( top, bottom );
      p.drawLine( left, right );
   }
}

} /// namespace Plotting
