#ifndef HORIZONTALSCROLLPAINTAREA_H
#define HORIZONTALSCROLLPAINTAREA_H

#include "ScrollPaintArea.h"

namespace Plotting
{

/**
 * @class HorizontalScrollPaintArea
 * @brief Implements a scroll area for horizontal scrolling, i.e. oriented along the x-axis.
 */
class HorizontalScrollPaintArea : public ScrollPaintArea
{
   Q_OBJECT

   public:
      /**
       * Constructor.
       */
      HorizontalScrollPaintArea( QWidget* parent = 0 );

   /**
    * @see ScrollPaintArea interface for docuemtation.
    */
   private:
      QRectF getDataRangeRect() const;
      QRectF getViewRangeRect() const;
      void updateViewportGraphFromShift( const QPointF& shift );
      double project( const QPointF& p ) const;
      QPointF getCanVecAlong() const;
      QPointF getCanVecOrthogonal() const;
      QPointF getCanVecOrthogonalUnitVector() const;

   /**
    * @see ScrollPaintArea interface for docuemtation.
    */
   private slots:
      virtual void viewportGraphChanged( const QRectF& newViewportOfGraph );
};

} /// namespace Plotting

#endif // HORIZONTALSCROLLPAINTAREA_H
