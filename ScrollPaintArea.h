#ifndef SCROLLPAINTAREA_H
#define SCROLLPAINTAREA_H

#include "PaintArea.h"

namespace Plotting
{

class ScrollPaintArea : public PaintAreaBase
{
   Q_OBJECT

   public:
      ScrollPaintArea( QWidget* parent );
      virtual ~ScrollPaintArea();

      void setDataRange( double min, double max );

   private:
      void paintEventImpl( QPaintEvent* paintEvent );

   private:
      virtual QRectF getDataRangeRect() const = 0;
      virtual QRectF getViewRangeRect() const = 0;

   private slots:
      virtual void viewPortChangedSlot( const QRectF& newViewPort ) = 0;

   protected:
      double   m_dataMin;
      double   m_dataMax;
      QRectF   m_viewPortGraph;

};

} /// namespace Plotting

#endif // SCROLLPAINTAREA_H
