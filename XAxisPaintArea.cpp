#include "XAxisPaintArea.h"

#include <QPainter>

namespace Plotting
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
XAxisPaintArea::XAxisPaintArea( QWidget* parent ) :
   AxisPaintArea( parent )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
XAxisPaintArea::~XAxisPaintArea()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getNumPixelsAlongAxis
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int XAxisPaintArea::getNumsPixelsAlongAxis()
{
   return m_canvas.width();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// drawMajorTick
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// drawMinorTick
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// drawAxisLine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void XAxisPaintArea::drawAxisLine()
{
   QPainter& p = getPainter();
   QLineF line( QPointF( m_canvas.left(), m_canvas.top() + 1 ), QPointF( m_canvas.right(), m_canvas.top() + 1 ) );
   p.setPen( QPen( QBrush( Qt::black ), 2 ) );
   p.drawLine( line );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMin
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double XAxisPaintArea::getMin() const
{
   return getViewport().left();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMax
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double XAxisPaintArea::getMax() const
{
   return getViewport().right();
}

} /// namespace Plotting