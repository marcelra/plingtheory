#ifndef ZSCATTERITEM_H
#define ZSCATTERITEM_H

#include "IYVsXItem.h"
#include "MarkerDrawAttr.h"
#include "Palette.h"

namespace Plotting
{

class ZScatterItem : public IYVsXItem, public MarkerDrawAttr
{
   public:
      ZScatterItem( const std::vector< double >& xData, const std::vector< double >& yData, const std::vector< double >& zData, const Palette& palette, const MarkerDrawAttr& markerDrawAttr = MarkerDrawAttr() );

   private:
      void generatePlotCommands( PaintArea* paintArea ) const;

   private:
      Palette							m_palette;
      std::vector< double >		m_zData;
      double							m_zMin;
      double							m_zMax;
};

} /// namespace Plotting

#endif // ZSCATTERITEM_H
