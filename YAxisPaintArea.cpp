#include "YAxisPaintArea.h"

#include <QPainter>

namespace Plotting
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
YAxisPaintArea::YAxisPaintArea( QWidget* parent ) :
   AxisPaintArea( parent )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getNumPixelsAlongAxis
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int YAxisPaintArea::getNumsPixelsAlongAxis()
{
   return m_canvas.height();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// drawMajorTick
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void YAxisPaintArea::drawMajorTick( double tickValue )
{
   /// Obtain the painter on this paint area.
   /// The YAxisPaintArea inherits a paint area base.
   QPainter& p = getPainter();

   /// Define size of major ticks.
   size_t tickLength = 10;

   /// Transform world-coordinates of tick position to canvas coordinates.
   QPointF tickPosCanvas = transformToCanvasCoordinates( QPointF( 0, tickValue ) );
   int tickPosY = tickPosCanvas.y();

   /// Build the line on the canvas of tick.
   QLineF tickLine( QPointF( m_canvas.right(), tickPosY ), QPointF( m_canvas.right() - tickLength, tickPosY ) );

   /// Draw the line.
   p.setPen( QPen( QBrush( Qt::black ), 2 ) );
   p.setRenderHint( QPainter::Antialiasing, true );
   p.drawLine( tickLine );

   /// Render the label.
   QString label = QString( "%1" ).arg( tickValue );

   QFont font;
   font.setPixelSize( 15 );
   p.setFont( font );

   p.save();
   p.translate( QPointF( m_canvas.right() + 1 - tickLength, tickPosY ) );

   QRectF textRectMax( QPoint( -110, -10 ), QSize( 100, 20 ) );
   p.drawText( textRectMax, Qt::AlignRight | Qt::AlignVCenter, label );

   p.restore();

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// drawMinorTick
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void YAxisPaintArea::drawMinorTick( double tickValue )
{
   QPainter& p = getPainter();
   size_t tickLength = 5;

   QPointF tickPosCanvas = transformToCanvasCoordinates( QPointF( 0, tickValue ) );
   int tickPosY = tickPosCanvas.y();

   QLineF tickLine( QPointF( m_canvas.right(), tickPosY ), QPointF( m_canvas.right() - tickLength, tickPosY ) );

   p.setPen( QPen( QBrush( Qt::black ), 1 ) );
   p.drawLine( tickLine );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// drawAxisLine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void YAxisPaintArea::drawAxisLine()
{
   QPainter& p = getPainter();
   QLineF line( QPointF( m_canvas.right(), m_canvas.top() ), QPointF( m_canvas.right(), m_canvas.bottom() ) );
   p.setPen( QPen( QBrush( Qt::black ), 2 ) );
   p.drawLine( line );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// drawAxisTitle
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void YAxisPaintArea::drawAxisTitle()
{
   QPainter& p = getPainter();

   size_t textMaxSize = m_canvas.height();

   QString label( m_title.c_str() );
   QRectF textRect( QPoint( -textMaxSize / 2, 0 ), QSize( textMaxSize, 30 ) );


   p.save();
   p.translate( QPointF( 0, m_canvas.height() / 2 ) );
   p.rotate( 270 );

   QColor textBkgColor( Qt::white );
   textBkgColor.setAlpha( 220 );
   QBrush brush( textBkgColor );

   p.save();
   p.setBrush( brush );
   p.setPen( QPen( brush, 0 ) );
   p.drawRect( textRect );
   p.restore();

   QFont font;
   font.setPixelSize( 15 );
   p.setFont( font );
   p.drawText( textRect, Qt::AlignHCenter | Qt::AlignVCenter, label );

   p.restore();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMin
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double YAxisPaintArea::getMin() const
{
   return getViewport().bottom();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMax
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double YAxisPaintArea::getMax() const
{
   return getViewport().top();
}

} /// namespace Plotting
