#ifndef SCATTERITEM_H
#define SCATTERITEM_H

#include "IYVsXItem.h"
#include "MarkerDrawAttr.h"

namespace Plotting
{

/**
 * @class ScatterItem
 * @brief A X vs Y scatter plot.
 */
class ScatterItem : public IYVsXItem, public MarkerDrawAttr
{
   public:
      ScatterItem( const std::vector< double >& xData, const std::vector< double >& yData, const MarkerDrawAttr& markerDrawAttr = MarkerDrawAttr() );
      virtual ~ScatterItem();

   private:
      void generatePlotCommands( PaintArea* paintArea ) const;
};

} /// namespace Plotting

#endif // SCATTERITEM_H
