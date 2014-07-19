#ifndef PAINTAREABASE_H
#define PAINTAREABASE_H

#include <QWidget>

#include <cassert>
#include <memory>

class QMouseEvent;

namespace Plotting
{

/**
 * @class PaintAreaBase
 * @brief Class describing a QWidget on which can be painted. Provides methods to perform coordinate transforms between
 * world coordinates and canvas coordinates. This transformation depends on the viewport.
 *
 * The world coordinates are (x, y) coordinates and translate to canvas coordinates through the viewport definition.
 * Canvas coordinates are reflected in the x-axis (i.e. y_canvas = -y_world). Note that the height of the viewport is
 * negative when high y points to high position on the screen (i.e. normal and desired situations).
 */
class PaintAreaBase : public QWidget
{
   Q_OBJECT

   public:
      /**
       * Constructor.
       */
      PaintAreaBase( QWidget* parent = 0 );
      /**
       * Virtual destructor.
       */
      virtual ~PaintAreaBase();

      /**
       * Transform world point @param point to canvas coordinates.
       */
      QPointF transformToCanvasCoordinates( const QPointF& point ) const;
      /**
       * Transform canvas point @param to world coordinates.
       */
      QPointF transformToWorldCoordinates( const QPoint& point ) const;

      /**
       * Sets viewport @param viewport (see @note about viewport's height).
       */
      virtual void setViewport( const QRectF& viewport );
      /**
       * Gets the viewport (see @note about viewport's height).
       */
      const QRectF& getViewport() const;
      /**
       * Gets the canvas.
       */
      const QRect& getCanvas() const;

      /**
       * Obtain a reference to the painter.
       */
      QPainter& getPainter();

      /**
       * Sets the name of the widget, not of the plot. This method is introduced mainly for debugging purposes.
       */
      void setName( const QString& name );

      /**
       * Clips viewport to data (i.e. cannot pan or zoom outside data range).
       */
      void setViewportConstraints( const QRectF& viewportConstraints );
      /**
       * Disable the viewport constraints if available.
       */
      void disableViewportConstraints();

   signals:
      /**
       * Signal that is emitted when the viewport is changed. The Plot2D class uses this to synchronise the different
       * viewports of the different paint areas.
       */
      void viewportChanged( const QRectF& newViewport );

   protected:
      /**
       * Handle a paint event. Constructs the painter and accepts the event at the end. Derived classes should do the actual
       * painting in paintEventImpl.
       */
      virtual void paintEvent( QPaintEvent* event );

      /**
       * Remove the old mouse position.
       */
      void mouseReleaseEvent( QMouseEvent* event );

      /**
       * Store the clicked mouse position.
       */
      void mousePressEvent( QMouseEvent* event );

      /**
       * The actual painting should be done in this method by the derived classes.
       */
      virtual void paintEventImpl( QPaintEvent* event ) = 0;

   private:
      void updateViewportWithConstraint( const QRectF& viewport );

   protected:
      QString                      m_name;
      QRectF                       m_viewport;
      QRect                        m_canvas;
      QPainter*                    m_painter;
      std::unique_ptr< QPoint >    m_oldMousePos;
      std::unique_ptr< QRectF >    m_viewportConstraints;      //! Viewport constraints. Cannot pan or zoom outside this area. Used for 2D histograms.
      std::unique_ptr< QPoint >    m_clickedMousePos;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods PaintAreaBase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getPainter
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline QPainter& PaintAreaBase::getPainter()
{
   assert( m_painter );
   return *m_painter;
}

} /// namespace Plotting

#endif // PAINTAREABASE_H
