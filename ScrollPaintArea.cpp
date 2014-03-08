#include "ScrollPaintArea.h"

#include <QBrush>
#include <QDebug>
#include <QPainter>
#include <QPen>

ScrollPaintArea::ScrollPaintArea( QWidget* parent ) :
   PaintAreaBase( parent ),
   m_dataMin( 0 ),
   m_dataMax( 0 )
{
   connect( this, SIGNAL( viewPortChanged( QRectF ) ), this, SLOT( viewPortChangedSlot( QRectF ) ) );
}

ScrollPaintArea::~ScrollPaintArea()
{}

void ScrollPaintArea::setDataRange( double min, double max )
{
   m_dataMin = min;
   m_dataMax = max;
}

void ScrollPaintArea::paintEventImpl( QPaintEvent* paintEvent )
{
   assert( paintEvent );

   QColor bkgColor( Qt::lightGray );
   bkgColor.setRgb( 220, 220, 220 );
   QBrush brush( bkgColor, Qt::SolidPattern );
   getPainter().setBrush( brush );
   getPainter().setPen( QPen( brush, 0 ) );
   getPainter().drawRect( m_canvas );

   QColor dataRangeColor( bkgColor );
   dataRangeColor.setRgb( 180, 180, 180 );
   brush.setColor( dataRangeColor );
   getPainter().setPen( QPen( brush, 0 ) );
   getPainter().setBrush( brush );
   getPainter().drawRect( getDataRangeRect() );

   QColor viewColor( Qt::blue );
   viewColor.setAlpha( 64 );
   brush.setColor( viewColor );
   getPainter().setBrush( brush );
   getPainter().setPen( QPen( brush, 2 ) );
   getPainter().drawRect( getViewRangeRect() );

}

