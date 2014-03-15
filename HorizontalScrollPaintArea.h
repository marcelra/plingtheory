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

   /**
    * @see ScrollPaintArea interface for docuemtation.
    */
   private slots:
      virtual void viewportChangedSlot( const QRectF& newViewportOfGraph );
};

} /// namespace Plotting

#endif // HORIZONTALSCROLLPAINTAREA_H
