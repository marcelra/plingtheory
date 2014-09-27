#ifndef YAXISPAINTAREA_H
#define YAXISPAINTAREA_H

#include "AxisPaintArea.h"

namespace Plotting
{

/**
 * @class YAxisPaintArea
 * @brief Implements an AxisPaintArea (@see AxisPaintArea). This is the visual representation of an Y-axis.
 */
class YAxisPaintArea : public AxisPaintArea
{
   Q_OBJECT

   public:
      /**
       * Constructor (@see AxisPaintArea).
       */
      YAxisPaintArea( QWidget* parent = 0 );

   public:
      /**
       * Get the bottom/lowest value displayed.
       */
      double getMin() const;
      /**
       * Get the top/highest value displayed.
       */
      double getMax() const;

   protected:
      /**
       * Draw an axis line.
       */
      void drawAxisLine();
      /**
       * Draw a single major tick (this includes drawing the value).
       */
      void drawMajorTick( double tickValue );
      /**
       * Draw a single minor tick.
       */
      void drawMinorTick( double tickValue );
      /**
       * Get the number of pixels on the canvas along the direction in which the axis points.
       */
      int getNumsPixelsAlongAxis();
      /**
       * Draws the axis title.
       */
      void drawAxisTitle();
      /**
       * drawKiloPower
       */
      void drawKiloPower();
};

} /// namespace Plotting

#endif // YAXISPAINTAREA_H
