#ifndef REGLARGEDATACURVE_H
#define REGLARGEDATACURVE_H

#include "IYVsXItem.h"
#include "LineDrawAttr.h"

namespace Plotting
{

/// TODO: doc
class RegLargeDataCurve : public IYVsXItem, public LineDrawAttr
{
   public:
      RegLargeDataCurve( const std::vector< double >& xData, const std::vector< double >& yData, size_t numPointsLowResCache = 10000 );
      virtual ~RegLargeDataCurve();

   private:
      void generatePlotCommands( PaintArea* paintArea ) const;

   private:
      void generateLowResCommands( PaintArea* paintArea ) const;
      void generateHiResCommands( PaintArea* paintArea ) const;

   private:
      YVsXData*      m_lowResCache;
      size_t         m_numPointsCache;
};

} /// namespace Plotting

#endif // REGLARGEDATACURVE_H
