#ifndef CURVEITEM_H
#define CURVEITEM_H

#include "IYVsXItem.h"
#include "LineDrawAttr.h"

namespace Plotting
{

/**
 * @class CurveItem
 * @brief PaintItem for displaying curves (i.e. graphs with lines).
 */
class CurveItem : public IYVsXItem, public LineDrawAttr
{
   public:
      /**
       * Constructor.
       */
      CurveItem( const std::vector< double >& xData, const std::vector< double >& yData );
      /**
       * Destructor.
       */
      virtual ~CurveItem();

   protected:
      CurveItem();

   private:
      /**
       * @see PaintItem.
       */
      void generatePlotCommands( PaintArea* paintArea ) const;

};

} /// namespace Plotting

#endif // CURVEITEM_H
