#ifndef SCATTERITEM_H
#define SCATTERITEM_H

#include "IYVsXItem.h"
#include "MarkerDrawAttr.h"

namespace Plotting
{

/// TODO: doc
class ScatterItem : public IYVsXItem, public MarkerDrawAttr
{
   public:
      ScatterItem( const std::vector< double >& xData, const std::vector< double >& yData );
      virtual ~ScatterItem();

   private:
      void generatePlotCommands( PaintArea* paintArea ) const;
};

} /// namespace Plotting

#endif // SCATTERITEM_H
