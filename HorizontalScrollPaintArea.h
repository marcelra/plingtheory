#ifndef HORIZONTALSCROLLPAINTAREA_H
#define HORIZONTALSCROLLPAINTAREA_H

#include "ScrollPaintArea.h"

namespace Plotting
{

/// TODO: doc
class HorizontalScrollPaintArea : public ScrollPaintArea
{
   Q_OBJECT

   public:
      HorizontalScrollPaintArea( QWidget* parent = 0 );
      virtual ~HorizontalScrollPaintArea();

   private:
      QRectF getDataRangeRect() const;
      QRectF getViewRangeRect() const;
      void updateViewPortGraphFromShift( const QPointF& shift );

   private slots:
      virtual void viewPortChangedSlot( const QRectF& newViewPort );
};

} /// namespace Plotting

#endif // HORIZONTALSCROLLPAINTAREA_H
