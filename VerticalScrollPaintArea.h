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

   /**
    * @see ScrollPaintArea interface for docuemtation.
    */
   private slots:
      virtual void viewportChangedSlot( const QRectF& newViewportOfGraph );
};

} /// namespace Plotting

#endif // VERTICALSCROLLPAINTAREA_H
