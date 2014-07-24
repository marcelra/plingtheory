#include "ScrollPaintArea.h"

#include "Palette.h"

#include <QBrush>
#include <QDebug>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPolygonF>
#include <QTimer>

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
/// ScrollBarAnimator private class
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct ScrollBarAnimator
{
   ScrollBarAnimator();

   void start();
   void nextFrame();

   std::unique_ptr< QTimer >   animationTimer;
   size_t                      animationFrameCounter;
   double                      animationProgess;
   static size_t               numTotalAnimations;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ScrollBarAnimator::ScrollBarAnimator() :
   animationTimer(),
   animationFrameCounter( 0 ),
   animationProgess( 1 )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// start
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ScrollBarAnimator::start()
{
   animationTimer.reset( new QTimer() );
   animationTimer->setInterval( 10 );
   animationFrameCounter = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// nextFrame
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ScrollBarAnimator::nextFrame()
{
   if ( animationFrameCounter == numTotalAnimations )
   {
      animationTimer.reset( 0 );
      animationTimer = 0;
      animationProgess = 1;
   }

   animationProgess = static_cast< double >( animationFrameCounter ) / numTotalAnimations;
   ++animationFrameCounter;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Static members ScrollBarAnimation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t ScrollBarAnimator::numTotalAnimations = 100;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ScrollPaintArea::ScrollPaintArea( QWidget* parent ) :
   PaintAreaBase( parent ),
   m_dataMin( 0 ),
   m_dataMax( 0 ),
   m_isScrolling( false ),
   m_scrollBarAnimator( new ScrollBarAnimator() )
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
   QAction* setMarkersAroundViewportAction = contextMenu.addAction( "Set markers around view" );
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
         m_originalDataMin = m_dataMin;
         m_originalDataMax = m_dataMax;
         m_marker0.reset( new double( posProjected ) );
         update();
      }
      else
      {
         m_marker1.reset( new double( posProjected ) );

         double markerMin = std::min( *m_marker0, *m_marker1 );
         double markerMax = std::max( *m_marker0, *m_marker1 );
         updateDataRange( markerMin, markerMax );

         emit updateViewportFromMarkers();
      }
   }
   else if ( action == setMarkersAroundViewportAction )
   {
      const std::pair< double, double >& boundingValues = getBoundingValuesViewportGraph();

      m_marker0.reset( new double ( boundingValues.first ) );
      m_marker1.reset( new double ( boundingValues.second ) );

      double markerMin = std::min( *m_marker0, *m_marker1 );
      double markerMax = std::max( *m_marker0, *m_marker1 );
      updateDataRange( markerMin, markerMax );

      emit updateViewportFromMarkers();
   }
   else if ( action == deleteMarkersAction )
   {
      m_marker0.reset( 0 );
      m_marker1.reset( 0 );
      m_dataMin = m_originalDataMin;
      m_dataMax = m_originalDataMax;
      emit updateViewportFromMarkers();
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ScrollPaintArea::~ScrollPaintArea()
{
   delete m_scrollBarAnimator;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setDataRange
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ScrollPaintArea::setDataRange( double min, double max )
{
   m_originalDataMin = min;
   m_originalDataMax = max;
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
   QColor bkgColor;
   bkgColor.setRgb( 220, 220, 220 );
   QBrush brush( bkgColor, Qt::SolidPattern );
   getPainter().setBrush( brush );
   getPainter().setPen( QPen( brush, 0 ) );
   getPainter().drawRect( m_canvas );

   double animationProgress = m_scrollBarAnimator->animationProgess;

   /// Draw data range.
   QColor dataRangeColor( bkgColor );
   dataRangeColor.setRgb( 180, 180, 180 );
   dataRangeColor.setAlpha( animationProgress * 255 );
   brush.setColor( dataRangeColor );
   getPainter().setPen( QPen( brush, 0 ) );
   getPainter().setBrush( brush );
   getPainter().drawRect( getDataRangeRect().toRect() );

   if ( m_marker0.get() )
   {
      drawMarker( *m_marker0 );
   }
   if ( m_marker1.get() )
   {
      drawMarker( *m_marker1 );
   }

   /// Draw scroll handle.
   std::vector< QColor > paletteStops;
   std::vector< double > stopPoints;
   stopPoints.push_back( 0 ); paletteStops.push_back( QColor( Qt::blue ) );
   stopPoints.push_back( 0.1 ); paletteStops.push_back( QColor( Qt::white ) );
   stopPoints.push_back( 1 ); paletteStops.push_back( QColor( Qt::blue ) );
   Plotting::Palette scrollPalette( paletteStops, stopPoints );

   QColor viewColor = scrollPalette.getColour( animationProgress );
   viewColor.setAlpha( 64 );
   brush.setColor( viewColor );
   getPainter().setBrush( brush );
   getPainter().setPen( QPen( brush, 2 ) );
   getPainter().drawRect( getViewRangeRect() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// drawMarker
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ScrollPaintArea::drawMarker( double markerPosition )
{
   const QPen& prevPen = getPainter().pen();
   const QBrush& prevBrush = getPainter().brush();

   QColor markerColour;

   if ( m_marker1.get() )
   {
      markerColour.setRgb( 80, 80, 80 );
   }
   else
   {
      markerColour.setRgb( 160, 0, 0 );
   }

   QBrush brush( markerColour, Qt::SolidPattern );
   QPointF markerProjected = markerPosition * getCanVecAlongUnitVector();
   QPointF pMinMarker = transformToCanvasCoordinates( markerProjected );
   pMinMarker = pointWiseMultiply( pMinMarker, getCanVecAlongUnitVector() );
   QPointF pMaxMarker = pMinMarker + getCanVecOrthogonal();

   size_t penSize = 2;

   getPainter().setPen( QPen( brush, penSize, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
   getPainter().drawLine( pMinMarker.toPoint(), pMaxMarker.toPoint() );

   double triangleSize = 4;
   QPointF triangleOrthogonal = triangleSize * getCanVecOrthogonalUnitVector();

   QPolygonF triangleUp;
   triangleUp.append( pMinMarker + triangleSize * getCanVecAlongUnitVector() );
   triangleUp.append( pMinMarker + triangleOrthogonal );
   triangleUp.append( pMinMarker - triangleSize * getCanVecAlongUnitVector() );

   QPolygonF triangleDown;
   triangleDown.append( pMaxMarker + triangleSize * getCanVecAlongUnitVector() );
   triangleDown.append( pMaxMarker - triangleOrthogonal );
   triangleDown.append( pMaxMarker - triangleSize * getCanVecAlongUnitVector() );

   getPainter().setPen( QPen( brush, penSize, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
   getPainter().setBrush( brush );
   getPainter().drawPolygon( triangleUp.toPolygon(), Qt::OddEvenFill );
   getPainter().drawPolygon( triangleDown.toPolygon(), Qt::OddEvenFill );

   getPainter().setPen( prevPen );
   getPainter().setBrush( prevBrush );
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
/// updateDataRange
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ScrollPaintArea::updateDataRange( double min, double max )
{
   m_dataMin = min;
   m_dataMax = max;
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// animateActive
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ScrollPaintArea::animateActive()
{
   m_scrollBarAnimator->start();
   connect( m_scrollBarAnimator->animationTimer.get(), SIGNAL( timeout() ), this, SLOT( animateTimerSlot() ) );
   m_scrollBarAnimator->animationTimer->start();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// animateTimerSlot
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ScrollPaintArea::animateTimerSlot()
{
   m_scrollBarAnimator->nextFrame();
   update();
}

} /// namespace Plotting
