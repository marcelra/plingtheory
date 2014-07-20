#ifndef VERTICALSCROLLPAINTAREA_H
#define VERTICALSCROLLPAINTAREA_H

#include "ScrollPaintArea.h"

namespace Plotting
{

/**
 * @class VerticalScrollPaintArea
 * @brief Implements a scroll area for vertical scrolling, oriented along the y-axis.
 */
class VerticalScrollPaintArea : public ScrollPaintArea
{
   Q_OBJECT

   public:
      /**
       * Constructor.
       */
      VerticalScrollPaintArea( QWidget* parent = 0 );

   /**
    * @see ScrollPaintArea interface for docuemtation.
    */
   private:
      QRectF getDataRangeRect() const;
      QRectF getViewRangeRect() const;
      void updateViewportGraphFromShift( const QPointF& shift );
      double project( const QPointF& p ) const;

      QPointF getCanVecAlong() const;
      QPointF getCanVecAlongUnitVector() const;
      QPointF getCanVecOrthogonal() const;
      QPointF getCanVecOrthogonalUnitVector() const;

   /**
    * @see ScrollPaintArea interface for docuemtation.
    */
   private slots:
      virtual void viewportGraphChanged( const QRectF& newViewportOfGraph );
};

} /// namespace Plotting

#endif // VERTICALSCROLLPAINTAREA_H
