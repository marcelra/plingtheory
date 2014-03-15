#ifndef GRIDITEM_H
#define GRIDITEM_H

#include "LineDrawAttr.h"
#include "IPaintItem.h"

#include <vector>

namespace Plotting
{

class AxisPaintArea;

/**
 * @class GridItem
 * PaintItem that draws a grid on a paint area. It uses AxisPaintArea objects to supply where the grid lines are. This
 * object contains references to the axis object. When these objects lose scope this object is invalid.
 */
class GridItem : public IPaintItem, public LineDrawAttr
{
   public:
      /**
       * Constructor.
       */
      GridItem( const AxisPaintArea& xAxis, const AxisPaintArea& yAxis );

      /**
       * @see IPaintItem::generatePlotCommands.
       */
      void generatePlotCommands( PaintArea* paintArea ) const;

      /**
       * @see IPaintItem methods.
       */
      double getMinX() const;
      double getMaxX() const;
      double getMinY() const;
      double getMaxY() const;

   private:
      const AxisPaintArea&      m_xAxis;        //! reference to the x-axis.
      const AxisPaintArea&      m_yAxis;        //! reference to the y-axis.
};

} /// namespace Plotting

#endif // GRIDITEM_H
