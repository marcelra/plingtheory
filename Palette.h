#ifndef PALETTE_H
#define PALETTE_H

#include <QColor>

namespace Plotting
{

/**
 * @class Palette
 * @brief Class describing a colour palette. The values in getColour should be normalised to [0,1].
 */
class Palette
{
   public:
      /**
       * Create a palette. At least two stops should be given at 0 and at 1.
       */
      Palette( const std::vector< QColor >& stops, const std::vector< double >& stopPoints );

      /**
       * Obtain colour from value z. Z should be normalised between 0 and 1.
       */
      QColor getColour( double z ) const;

      /**
       * Prefab palettes.
       */
      static Palette whiteBlackPalette();
      static Palette blueRedPalette();
      static Palette heatPalette();
      static Palette firePalette();
      static Palette rainbowPalette();
      static Palette inverseRainbowPalette();

   private:
      /**
       * Morph color @param c1 to color @param c2. The parameter @param x should be between 0 and 1.
       */
      QColor interpolate( const QColor& c1, const QColor& c2, double x ) const;

   private:
      std::vector< QColor >      m_stops;
      std::vector< double >      m_stopPoints;
};

} /// namespace Plotting

#endif // PALETTE_H
