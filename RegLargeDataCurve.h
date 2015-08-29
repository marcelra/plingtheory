#ifndef REGLARGEDATACURVE_H
#define REGLARGEDATACURVE_H

#include "CurveItem.h"
#include "IYVsXItem.h"
#include "LineDrawAttr.h"

#include <map>

namespace Plotting
{

/**
 * @class RegLargeDataCurve
 * @brief PlotItem for yData as function of regularly spaced xData. Adds cache functionality w.r.t. the curve item (@see CurveItem).
 */
class RegLargeDataCurve : public IYVsXItem, public LineDrawAttr
{
   public:
      RegLargeDataCurve( const std::vector< double >& xData, const std::vector< double >& yData );
      virtual ~RegLargeDataCurve();

   private:
      void generatePlotCommands( PaintArea* paintArea ) const;

      const YVsXData& getData( int compressionFactor ) const;
      void createLowResDataSet( int compressionFactor ) const;

   public:
      mutable std::map< size_t, YVsXData >      m_cache;

};

} /// namespace Plotting

#endif // REGLARGEDATACURVE_H
