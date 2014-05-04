#ifndef PALETTE_H
#define PALETTE_H

#include <QColor>

namespace Plotting
{

class Palette
{
   public:
      Palette( const std::vector< QColor >& stops, const std::vector< double >& stopPoints );

      QColor getColour( double z ) const;

      static Palette whiteBlackPalette();
      static Palette blueRedPalette();
      static Palette heatPalette();

   private:
      QColor interpolate( const QColor& c1, const QColor& c2, double x ) const;

   private:
      std::vector< QColor >      m_stops;
      std::vector< double >      m_stopPoints;
};

} /// namespace Plotting

#endif // PALETTE_H
