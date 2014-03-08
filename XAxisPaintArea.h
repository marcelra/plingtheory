#ifndef XAXISPAINTAREA_H
#define XAXISPAINTAREA_H

#include "AxisPaintArea.h"

class XAxisPaintArea : public AxisPaintArea
{
   Q_OBJECT

   public:
      XAxisPaintArea( QWidget* parent = 0 );
      virtual ~XAxisPaintArea();

   public:
      double getMin() const;
      double getMax() const;

   protected:
      void drawAxisLine();
      void drawMajorTick( double tickValue );
      void drawMinorTick( double tickValue );
      int getNumsPixelsAlongAxis();
};

#endif // XAXISPAINTAREA_H
