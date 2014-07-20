#include "ScrollPaintArea.h"

#include <QBrush>
#include <QDebug>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPolygonF>

namespace
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// pointWiseMultiply
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QPointF pointWiseMultiply( const QPointF& p1, const QPointF& p2 )
{
   QPointF result;
   result.setX( p1.x() * p2.x() );
   result.setY( p1.y() * p2.y() );
   return result;
}

} /// anonymous namespace

namespace Plotting
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ScrollPaintArea::ScrollPaintArea( QWidget* parent ) :
   PaintAreaBase( parent ),
   m_dataMin( 0 ),
   m_dataMax( 0 ),
   m_isScrolling( false )
{
   setContextMenuPolicy( Qt::CustomContextMenu );
   connect( this, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( showContextMenu( const QPoint& ) ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// showContextMenu
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ScrollPaintArea::showContextMenu( const QPoint& pos )
{
   QPoint globalPos = mapToGlobal( pos );

   QMenu contextMenu;
   QAction* setMarkerAction = contextMenu.addAction(  "Set marker" );
   QAction* deleteMarkersAction = contextMenu.addAction( "Delete markers" );

   if ( m_marker0.get() && m_marker1.get() )
   {
      setMarkerAction->setEnabled( false );
   }

   QAction* action = contextMenu.exec( globalPos );
   if ( action == setMarkerAction )
   {
      double posProjected = project( transformToWorldCoordinates( pos ) );
      if ( !m_marker0.get() )
      {
         m_marker0.reset( new double( posProjected ) );
         update();
      }
      else
      {
         m_marker1.reset( new double( posProjected ) );
         m_dataMinOld = m_dataMin;
         m_dataMaxOld = m_dataMax;
         double markerMin = std::min( *m_marker0, *m_marker1 );
         double markerMax = std::max( *m_marker0, *m_marker1 );
         setDataRange( markerMin, markerMax );
         emit updateViewportFromMarkers();
      }
   }
   else if ( action == deleteMarkersAction )
   {
      m_marker0.reset( 0 );
      m_marker1.reset( 0 );
      m_dataMin = m_dataMinOld;
      m_dataMax = m_dataMaxOld;
      update();
      emit updateViewportFromMarkers();
   }
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
   getPainter().drawRect( getDataRangeRect().toRect() );

   /// Draw scroll handle.
   QColor viewColor( Qt::blue );
   viewColor.setAlpha( 64 );
   brush.setColor( viewColor );
   getPainter().setBrush( brush );
   getPainter().setPen( QPen( brush, 2 ) );
   getPainter().drawRect( getViewRangeRect() );

   if ( m_marker0.get() )
   {
      drawMarker( *m_marker0 );
   }
   if ( m_marker1.get() )
   {
      drawMarker( *m_marker1 );
   }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// drawMarker
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ScrollPaintArea::drawMarker( double markerPosition )
{
   const QPen& prevPen = getPainter().pen();
   const QBrush& prevBrush = getPainter().brush();
   QPainter::RenderHints prevRenderHints = getPainter().renderHints();

   getPainter().setRenderHint( QPainter::Antialiasing, false );

   QBrush brush( Qt::darkBlue, Qt::SolidPattern );
   QPointF markerProjected = markerPosition * getCanVecAlong();
   QPointF pMinMarker = transformToCanvasCoordinates( markerProjected );
   pMinMarker = pointWiseMultiply( pMinMarker, getCanVecAlong() );
   QPointF pMaxMarker = pMinMarker + getCanVecOrthogonal();

   if ( pMinMarker.manhattanLength() < 1 )
   {
      pMinMarker += getCanVecAlong();
      pMaxMarker += getCanVecAlong();
   }

   pMinMarker += getCanVecOrthogonalUnitVector();

   getPainter().setPen( QPen( brush, 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
   getPainter().drawLine( pMinMarker.toPoint(), pMaxMarker.toPoint() );

   double triangleSize = 4;
   QPointF triangleOrthogonal = triangleSize * getCanVecOrthogonalUnitVector();

   QPolygonF triangleUp;
   triangleUp.append( pMinMarker + triangleSize * getCanVecAlong() );
   triangleUp.append( pMinMarker + triangleOrthogonal );
   triangleUp.append( pMinMarker - triangleSize * getCanVecAlong() );

   QPolygonF triangleDown;
   triangleDown.append( pMaxMarker + triangleSize * getCanVecAlong() );
   triangleDown.append( pMaxMarker - triangleOrthogonal );
   triangleDown.append( pMaxMarker - triangleSize * getCanVecAlong() );

   getPainter().setPen( QPen( brush, 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
   getPainter().setBrush( brush );
   getPainter().drawPolygon( triangleUp.toPolygon(), Qt::OddEvenFill );
   getPainter().drawPolygon( triangleDown.toPolygon(), Qt::OddEvenFill );

   getPainter().setPen( prevPen );
   getPainter().setBrush( prevBrush );
   getPainter().setRenderHints( prevRenderHints );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// mouseMoveEvent
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ScrollPaintArea::mouseMoveEvent( QMouseEvent* event )
{
   /// Store old mouse pos if it does not exist.
   if ( !m_oldMousePos.get() )
   {
      m_oldMousePos.reset( new QPoint( event->pos() ) );
      event->accept();
      return;
   }

   /// Notify that user is scrolling (this should freezes the viewport).
   setIsScrolling( true );

   /// Calculate shift in world coordinates.
   const QPointF& shiftOfViewport = transformToWorldCoordinates( event->pos() ) - transformToWorldCoordinates( *m_oldMousePos );

   /// Modify current viewport.
   updateViewportGraphFromShift( shiftOfViewport );

   /// Apply new viewport.
   emit viewportFromScroll( m_viewportGraph );

   /// Update mouse position.
   m_oldMousePos.reset( new QPoint( event->pos() ) );

   /// Notify that user has finished scrolling.
   setIsScrolling( false );

   /// Set event handled.
   event->accept();

   update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// blockUpdateViewport
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ScrollPaintArea::setIsScrolling( bool isScrolling )
{
   m_isScrolling = isScrolling;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// viewportUpdateIsBlocked
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ScrollPaintArea::isScrolling() const
{
   return m_isScrolling;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setViewport
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ScrollPaintArea::setViewport( const QRectF& viewport )
{
   /// Only update the viewport of the graph when the user is scrolling.
   if ( isScrolling() )
   {
      m_viewportGraph = viewport;
   }
   else
   {
      m_viewport = viewport;
      m_viewportGraph = viewport;
      viewportGraphChanged( viewport );
   }
   update();
}

} /// namespace Plotting
