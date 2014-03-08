#include "XAxisPaintArea.h"

#include <QPainter>

namespace Plotting
{

XAxisPaintArea::XAxisPaintArea( QWidget* parent ) :
   AxisPaintArea( parent )
{}

XAxisPaintArea::~XAxisPaintArea()
{}

int XAxisPaintArea::getNumsPixelsAlongAxis()
{
   return m_canvas.width();
}

void XAxisPaintArea::drawMajorTick( double tickValue )
{
   QPainter& p = getPainter();

   size_t tickLength = 10;

   QPointF tickPosCanvas = transformToCanvasCoordinates( QPointF( tickValue, 0 ) );
   int tickPosX = tickPosCanvas.x();

   QLineF tickLine( QPointF( tickPosX, 0 ), QPointF( tickPosX, tickLength ) );

   p.setPen( QPen( QBrush( Qt::black ), 2 ) );
   p.setRenderHint( QPainter::Antialiasing, true );
   p.drawLine( tickLine );

   QString label = QString( "%1" ).arg( tickValue );

   // QFont font( "Ubuntu Mono" );
   QFont font;
   font.setPixelSize( 15 );
   p.setFont( font );

   p.save();
   p.translate( QPointF( tickPosX, tickLength ) );
   p.rotate( 45 );

   QRectF textRectMax( QPoint( 10, -10 ), QSize( 100, 20 ) );
   p.drawText( textRectMax, Qt::AlignVCenter, label );

   p.restore();

}

void XAxisPaintArea::drawMinorTick( double tickValue )
{
   QPainter& p = getPainter();
   size_t tickLength = 5;

   QPointF tickPosCanvas = transformToCanvasCoordinates( QPointF( tickValue, 0 ) );
   int tickPosX = tickPosCanvas.x();

   QLineF tickLine( QPointF( tickPosX, 0 ), QPointF( tickPosX, tickLength ) );

   p.setPen( QPen( QBrush( Qt::black ), 1 ) );
   p.drawLine( tickLine );
}

void XAxisPaintArea::drawAxisLine()
{
   QPainter& p = getPainter();
   QLineF line( QPointF( m_canvas.left(), m_canvas.top() + 1 ), QPointF( m_canvas.right(), m_canvas.top() + 1 ) );
   p.setPen( QPen( QBrush( Qt::black ), 2 ) );
   p.drawLine( line );
}

double XAxisPaintArea::getMin() const
{
   return getViewPort().left();
}

double XAxisPaintArea::getMax() const
{
   return getViewPort().right();
}

} /// namespace Plotting
