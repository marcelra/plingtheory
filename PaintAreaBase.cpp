#include "PaintAreaBase.h"

#include <cmath>

#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>

#include <QDebug>

namespace Plotting
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PaintAreaBase::PaintAreaBase( QWidget* parent ) :
   QWidget( parent ),
   m_name( "Undefined" ),
   m_oldMousePos( 0 ),
   m_viewportConstraints( 0 )
{
   setBackgroundRole( QPalette::Base );
   setAutoFillBackground( true );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PaintAreaBase::~PaintAreaBase()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// transformToCanvasCoordinates
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QPointF PaintAreaBase::transformToCanvasCoordinates( const QPointF& point ) const
{
   QPointF result;

   double scaleX = m_canvas.width() / m_viewport.width();
   double scaleY = - m_canvas.height() / m_viewport.height();

   double shiftX = -m_viewport.left();

   result.setX( ( point.x() + shiftX ) * scaleX );
   result.setY( scaleY * ( m_viewport.top() - point.y() ) );

   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// transformToWorldCoordinates
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QPointF PaintAreaBase::transformToWorldCoordinates( const QPoint& point ) const
{
   double scaleX = m_viewport.width() / m_canvas.width();
   double scaleY = -m_viewport.height() / m_canvas.height();

   double shiftX = m_viewport.left();

   QPointF result;
   result.setX( point.x() * scaleX + shiftX );
   result.setY( -scaleY * point.y() + m_viewport.top() );

   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// paintEvent
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintAreaBase::paintEvent( QPaintEvent* paintEvent )
{
   /// Synchronise canvas size.
   m_canvas = QRect( 0, 0, width() - 1, height() - 1 );

   /// Contruct painter.
   m_painter = new QPainter( this );

   /// Call implementation specific paint method.
   paintEventImpl( paintEvent );

   /// Cleanup.
   delete m_painter;

   /// Accept the event.
   paintEvent->accept();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setName
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintAreaBase::setName( const QString& name )
{
   m_name = name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getViewport
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const QRectF& PaintAreaBase::getViewport() const
{
   return m_viewport;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setViewport
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintAreaBase::setViewport( const QRectF& viewport )
{
   m_viewport = viewport;
   if ( m_viewportConstraints.get() )
   {
      updateViewportWithConstraint( viewport );
   }
   emit viewportChanged( m_viewport );
   update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// mouseReleaseEvent
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintAreaBase::mouseReleaseEvent( QMouseEvent* event )
{
   if ( m_oldMousePos.get() )
   {
      m_oldMousePos.reset( 0 );
   }
   event->accept();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getCanvas
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const QRect& PaintAreaBase::getCanvas() const
{
   return m_canvas;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setClipViewportToData
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintAreaBase::setViewportConstraints( const QRectF& viewportConstraints )
{
   m_viewportConstraints.reset( new QRectF( viewportConstraints ) );
   setViewport( viewportConstraints );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// disableViewportConstraints
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintAreaBase::disableViewportConstraints()
{
   m_viewportConstraints.reset( 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getWithinConstraints
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PaintAreaBase::updateViewportWithConstraint( const QRectF& viewport )
{
   double newWidth = viewport.width();
   if ( newWidth > m_viewportConstraints->width() )
   {
      newWidth = m_viewportConstraints->width();
   }

   double newHeight = fabs( viewport.height() );
   if ( newHeight > fabs( m_viewportConstraints->height() ) )
   {
      newHeight = fabs( m_viewportConstraints->height() );
   }

   if ( viewport.left() < m_viewportConstraints->left() )
   {
      m_viewport.setLeft( m_viewportConstraints->left() );
      m_viewport.setRight( m_viewport.left() + newWidth );
   }
   if ( viewport.right() > m_viewportConstraints->right() )
   {
      m_viewport.setRight( m_viewportConstraints->right() );
      m_viewport.setLeft( m_viewport.right() - newWidth );
   }
   if ( viewport.bottom() < m_viewportConstraints->bottom() )
   {
      m_viewport.setBottom( m_viewportConstraints->bottom() );
      m_viewport.setTop( m_viewport.bottom() + newHeight );
   }
   if ( viewport.top() > m_viewportConstraints->top() )
   {
      m_viewport.setTop( m_viewportConstraints->top() );
      m_viewport.setBottom( m_viewport.top() - newHeight );
   }
}

} /// namespace Plotting

