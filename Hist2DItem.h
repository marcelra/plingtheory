#ifndef HIST2DITEM_H
#define HIST2DITEM_H

#include "IPaintItem.h"

#include <QPixmap>

namespace Math
{
class Regular2DHistogram;
}

namespace Plotting
{
class Palette;
}

namespace Plotting
{

/**
 * @class Hist2DItem
 * @brief PaintItem for two dimensional histogram with regular sized bins. @see IPaintItem.
 */
class Hist2DItem : public IPaintItem
{
   public:
      /**
       * Create a Hist2DItem from a Regular2DHistogram @param hist2D.
       */
      Hist2DItem( const Math::Regular2DHistogram& hist2D, const Palette& palette );

      double getMinX() const;
      double getMaxX() const;
      double getMinY() const;
      double getMaxY() const;

      void generatePlotCommands( PaintArea* paintArea ) const;

   private:
      QPixmap     m_pixmap;
      double      m_minX;
      double      m_maxX;
      double      m_minY;
      double      m_maxY;
};

} /// namespace Plotting

#endif // HIST2DITEM_H
