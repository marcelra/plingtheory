#ifndef SCROLLPAINTAREA_H
#define SCROLLPAINTAREA_H

#include "PaintArea.h"

namespace Plotting
{

/**
 * @class ScrollPaintArea
 * @brief Base class for scroll bars that visually indicate the range of the data and the viewport.
 *
 * The viewport of the ScrollPaintArea is the maximum range that can be made from the viewport and the datarange. This means
 * that the viewport of the graph and the datarange are always in the viewport of this paintarea. Unlike the axis objects, the viewport
 * of the scroll paint area is not identical to the viewport of the graph. Naturally, when the viewport of the graph is changed,
 * this should be notified to this class as well.
 */
class ScrollPaintArea : public PaintAreaBase
{
   Q_OBJECT

   public:
      /**
       * Constructor.
       */
      ScrollPaintArea( QWidget* parent );
      /**
       * Descructor.
       */
      virtual ~ScrollPaintArea();

      /**
       * Sets the range of the data for this scroll bar.
       */
      void setDataRange( double min, double max );

   protected:
      /**
       * True if updates from viewport are blocked.
       */
      bool isScrolling() const;

   private:
      /**
       * @see PaintAreaBase::paintEventImpl.
       */
      void paintEventImpl( QPaintEvent* paintEvent );
      /**
       * Handle mouse event.
       */
      void mouseMoveEvent( QMouseEvent* event );

   private:
      /**
       * Calculate the canvas rectangle displaying the range of the data.
       */
      virtual QRectF getDataRangeRect() const = 0;
      /**
       * Calculate the canvas rectangle displaying the range of the viewport.
       */
      virtual QRectF getViewRangeRect() const = 0;
      /**
       * Given a shift of the viewport from e.g. a mouse move event, update the graph view port.
       */
      virtual void updateViewportGraphFromShift( const QPointF& shift ) = 0;
      /**
       * Block updating the viewport.
       */
      void setIsScrolling( bool isScrolling );

   signals:
      /**
       * Signals that the viewport has been changed by a scroll action.
       */
      void viewportFromScroll( const QRectF& viewport );

   public:
      /**
       * Sets viewport. Can be graph viewport only if user is scrolling.
       */
      void setViewport( const QRectF& viewport );

   private slots:
      /**
       * Handle a view port change of the graph.
       */
      virtual void viewportGraphChanged( const QRectF& newViewportOfGraph ) = 0;

   protected:
      double   m_dataMin;                 //! Min of the data.
      double   m_dataMax;                 //! Max of the data.
      QRectF   m_viewportGraph;           //! Viewport of the graph.
      bool     m_isScrolling;             //! True iff user is scrolling.

};

} /// namespace Plotting

#endif // SCROLLPAINTAREA_H
