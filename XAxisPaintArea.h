#ifndef XAXISPAINTAREA_H
#define XAXISPAINTAREA_H

#include "AxisPaintArea.h"

namespace Plotting
{

/**
 * @class XAxisPaintArea
 * @brief Class deriving from AxisPaintArea with implementation for drawing an x-axis. See for more detailed documentation
 * @see YAxisPaintArea and @see AxisPaintArea.
 */
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
      void drawAxisTitle();
};

} /// namespace Plotting

#endif // XAXISPAINTAREA_H
