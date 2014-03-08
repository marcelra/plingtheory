#ifndef GRIDITEM_H
#define GRIDITEM_H

#include "LineDrawAttr.h"
#include "IPaintItem.h"

#include <vector>

namespace Plotting
{

class AxisPaintArea;

class GridItem : public IPaintItem, public LineDrawAttr
{
   public:
      GridItem( const AxisPaintArea& xAxis, const AxisPaintArea& yAxis );
      virtual ~GridItem();

      void updateGrid( const AxisPaintArea& xAxis, const AxisPaintArea& yAxis );
      void generatePlotCommands( PaintArea* paintArea ) const;

      double getMinX() const;
      double getMaxX() const;
      double getMinY() const;
      double getMaxY() const;

   private:
      const AxisPaintArea&      m_xAxis;
      const AxisPaintArea&      m_yAxis;
};

} /// namespace Plotting

#endif // GRIDITEM_H
