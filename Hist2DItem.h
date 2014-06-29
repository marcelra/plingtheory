#ifndef HIST2DITEM_H
#define HIST2DITEM_H

#include "IPaintItem.h"
#include "Palette.h"

#include <QPixmap>

namespace Math
{
class Regular2DHistogram;
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
      void createPixmaps( const Math::Regular2DHistogram& hist2D, const Palette& palette );
      const QPixmap& getPixmap( double compressionX, double compressionY ) const;

      void fillPixmap( QPixmap& pixmap, size_t xCompIndex, size_t yCompIndex ) const;
      QRgb getMaxColourInRect( const QPoint& pos, const QSize& rect ) const;

   private:
      std::vector< std::vector< double > >                    m_relPixmap;
      double                                                  m_minX;
      double                                                  m_maxX;
      double                                                  m_minY;
      double                                                  m_maxY;
      mutable std::map< size_t, std::map< size_t, QPixmap > > m_cache;
      Palette                                                 m_palette;
};

} /// namespace Plotting

#endif // HIST2DITEM_H
