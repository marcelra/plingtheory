#include "ScrollPaintArea.h"

#include <QBrush>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>

namespace Plotting
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ScrollPaintArea::ScrollPaintArea( QWidget* parent ) :
   PaintAreaBase( parent ),
   m_dataMin( 0 ),
   m_dataMax( 0 )
{
   connect( this, SIGNAL( viewportChanged( QRectF ) ), this, SLOT( viewportChangedSlot( QRectF ) ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ScrollPaintArea::~ScrollPaintArea()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setDataRange
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ScrollPaintArea::setDataRange( double min, double max )
{
   m_dataMin = min;
   m_dataMax = max;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// paintEventImpl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ScrollPaintArea::paintEventImpl( QPaintEvent* paintEvent )
{
   assert( paintEvent );

   /// Draw background color.
   QColor bkgColor( Qt::lightGray );
   bkgColor.setRgb( 220, 220, 220 );
   QBrush brush( bkgColor, Qt::SolidPattern );
   getPainter().setBrush( brush );
   getPainter().setPen( QPen( brush, 0 ) );
   getPainter().drawRect( m_canvas );

   /// Draw data range.
   QColor dataRangeColor( bkgColor );
   dataRangeColor.setRgb( 180, 180, 180 );
   brush.setColor( dataRangeColor );
   getPainter().setPen( QPen( brush, 0 ) );
   getPainter().setBrush( brush );
   getPainter().drawRect( getDataRangeRect() );

   /// Draw scroll handle.
   QColor viewColor( Qt::blue );
   viewColor.setAlpha( 64 );
   brush.setColor( viewColor );
   getPainter().setBrush( brush );
   getPainter().setPen( QPen( brush, 2 ) );
   getPainter().drawRect( getViewRangeRect() );

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// mouseMoveEvent
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
   updateViewportGraphFromShift( shiftOfViewport );

   /// Apply new viewport.
   emit viewportFromScroll( m_viewportGraph );

   /// Update mouse position.
   *m_oldMousePos = event->pos();

   /// Set event handled.
   event->accept();

   update();
}

} /// namespace Plotting
