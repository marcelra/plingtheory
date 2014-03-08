#ifndef VERTICALSCROLLPAINTAREA_H
#define VERTICALSCROLLPAINTAREA_H

#include "ScrollPaintArea.h"

class VerticalScrollPaintArea : public ScrollPaintArea
{
   Q_OBJECT

   public:
      VerticalScrollPaintArea( QWidget* parent = 0 );
      virtual ~VerticalScrollPaintArea();

   private:
      virtual QRectF getDataRangeRect() const;
      virtual QRectF getViewRangeRect() const;

   private slots:
      virtual void viewPortChangedSlot( const QRectF& newViewPort );
};

#endif // VERTICALSCROLLPAINTAREA_H
