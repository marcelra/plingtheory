#ifndef CURVEITEM_H
#define CURVEITEM_H

#include "IYVsXItem.h"
#include "LineDrawAttr.h"

#include <QColor>

#include <vector>

namespace Plotting
{

class CurveItem : public IYVsXItem, public LineDrawAttr
{
   public:
      CurveItem( const std::vector< double >& xData, const std::vector< double >& yData );
      virtual ~CurveItem();

   private:
      void generatePlotCommands( PaintArea* paintArea ) const;

};

} /// namespace Plotting

#endif // CURVEITEM_H
