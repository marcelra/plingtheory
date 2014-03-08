#include "YAxisPaintArea.h"

#include <QPainter>

namespace Plotting
{

YAxisPaintArea::YAxisPaintArea( QWidget* parent ) :
   AxisPaintArea( parent )
{}

YAxisPaintArea::~YAxisPaintArea()
{}

int YAxisPaintArea::getNumsPixelsAlongAxis()
{
   return m_canvas.height();
}

void YAxisPaintArea::drawMajorTick( double tickValue )
{
   QPainter& p = getPainter();

   size_t tickLength = 10;

   QPointF tickPosCanvas = transformToCanvasCoordinates( QPointF( 0, tickValue ) );
   int tickPosY = tickPosCanvas.y();

   QLineF tickLine( QPointF( m_canvas.right(), tickPosY ), QPointF( m_canvas.right() - tickLength, tickPosY ) );

   p.setPen( QPen( QBrush( Qt::black ), 2 ) );
   p.setRenderHint( QPainter::Antialiasing, true );
   p.drawLine( tickLine );

   QString label = QString( "%1" ).arg( tickValue );

   QFont font;
   font.setPixelSize( 15 );
   p.setFont( font );

   p.save();
   p.translate( QPointF( m_canvas.right() + 1 - tickLength, tickPosY ) );
   // p.rotate( 45 );

   QRectF textRectMax( QPoint( -110, -10 ), QSize( 100, 20 ) );
   p.drawText( textRectMax, Qt::AlignRight | Qt::AlignVCenter, label );

   p.restore();

}

void YAxisPaintArea::drawMinorTick( double tickValue )
{
   QPainter& p = getPainter();
   size_t tickLength = 5;

   QPointF tickPosCanvas = transformToCanvasCoordinates( QPointF( 0, tickValue ) );
   int tickPosY = tickPosCanvas.y();

   QLineF tickLine( QPointF( m_canvas.right() + 1, tickPosY ), QPointF( m_canvas.right() - tickLength + 1, tickPosY ) );

   p.setPen( QPen( QBrush( Qt::black ), 1 ) );
   p.drawLine( tickLine );
}

void YAxisPaintArea::drawAxisLine()
{
   QPainter& p = getPainter();
   QLineF line( QPointF( m_canvas.right() + 1, m_canvas.top() ), QPointF( m_canvas.right() + 1, m_canvas.bottom() ) );
   p.setPen( QPen( QBrush( Qt::black ), 2 ) );
   p.drawLine( line );
}

double YAxisPaintArea::getMin() const
{
   return getViewPort().top();
}

double YAxisPaintArea::getMax() const
{
   return getViewPort().bottom();
}

} /// namespace Plotting
