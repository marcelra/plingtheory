#ifndef VERTICALSCROLLPAINTAREA_H
#define VERTICALSCROLLPAINTAREA_H

#include "ScrollPaintArea.h"

namespace Plotting
{

class VerticalScrollPaintArea : public ScrollPaintArea
{
   Q_OBJECT

   public:
      VerticalScrollPaintArea( QWidget* parent = 0 );
      virtual ~VerticalScrollPaintArea();

   private:
      QRectF getDataRangeRect() const;
      QRectF getViewRangeRect() const;
      void updateViewPortGraphFromShift( const QPointF& shift );

   private slots:
      virtual void viewPortChangedSlot( const QRectF& newViewPort );
};

} /// namespace Plotting

#endif // VERTICALSCROLLPAINTAREA_H
