#include "ScrollPaintArea.h"

#include <QBrush>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>

namespace Plotting
{

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

void ScrollPaintArea::mouseMoveEvent( QMouseEvent* event )
{
   /// Store old mouse pos if it does not exist.
   if ( !m_oldMousePos )
   {
      m_oldMousePos = new QPoint( event->pos() );
      event->accept();
      return;
   }

   /// Calculate shift in world coordinates.
   const QPointF& shiftOfViewport = transformToWorldCoordinates( event->pos() ) - transformToWorldCoordinates( *m_oldMousePos );

   /// Modify current viewport.
   // QRectF viewPort = m_viewPortGraph;
   updateViewPortGraphFromShift( shiftOfViewport );

   /// Apply new viewport.
   emit viewPortFromScroll( m_viewPortGraph );

   /// Update mouse position.
   *m_oldMousePos = event->pos();

   /// Set event handled.
   event->accept();

   update();
}

} /// namespace Plotting
