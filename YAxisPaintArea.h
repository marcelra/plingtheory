#ifndef YAXISPAINTAREA_H
#define YAXISPAINTAREA_H

#include "AxisPaintArea.h"

class YAxisPaintArea : public AxisPaintArea
{
   Q_OBJECT

   public:
      YAxisPaintArea( QWidget* parent = 0 );
      virtual ~YAxisPaintArea();

   public:
      double getMin() const;
      double getMax() const;

   protected:
      void drawAxisLine();
      void drawMajorTick( double tickValue );
      void drawMinorTick( double tickValue );
      int getNumsPixelsAlongAxis();
};

#endif // YAXISPAINTAREA_H
