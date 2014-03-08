#ifndef HORIZONTALSCROLLPAINTAREA_H
#define HORIZONTALSCROLLPAINTAREA_H

#include "ScrollPaintArea.h"

class HorizontalScrollPaintArea : public ScrollPaintArea
{
   Q_OBJECT

   public:
      HorizontalScrollPaintArea( QWidget* parent = 0 );
      virtual ~HorizontalScrollPaintArea();

   private:
      virtual QRectF getDataRangeRect() const;
      virtual QRectF getViewRangeRect() const;

   private slots:
      virtual void viewPortChangedSlot( const QRectF& newViewPort );
};

#endif // HORIZONTALSCROLLPAINTAREA_H
