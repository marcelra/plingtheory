#ifndef HIST2DITEM_H
#define HIST2DITEM_H

#include "IPaintItem.h"

#include "Regular2DHistogram.h"

#include <QPixmap>

namespace Plotting
{

class Hist2DItem : public IPaintItem
{
   public:
      Hist2DItem( const Math::Regular2DHistogram& hist2D );

      double getMinX() const;
      double getMaxX() const;
      double getMinY() const;
      double getMaxY() const;

      void generatePlotCommands( PaintArea* paintArea ) const;

   private:
      Math::Regular2DHistogram   m_hist;
      QPixmap                    m_pixmap;
};

} /// namespace Plotting

#endif // HIST2DITEM_H
