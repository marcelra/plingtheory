#include "Palette.h"

#include <QDebug>

#include <cassert>
#include <cmath>

namespace Plotting
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Palette::Palette( const std::vector< QColor >& stops, const std::vector< double >& stopPoints ) :
   m_stops( stops ),
   m_stopPoints( stopPoints ),
   m_colourCache( s_numColoursInCache )
{
   assert( stopPoints[0] == 0 );
   assert( stopPoints[ stopPoints.size() - 1 ] == 1 );
   assert( stopPoints.size() == m_stops.size() );
   assert( stopPoints.size() >= 2 );

   for ( size_t iColor = 0; iColor < s_numColoursInCache; ++iColor )
   {
      double z = static_cast< double >( iColor ) / s_numColoursInCache;

      size_t stopIndexLeft = 0;
      for ( size_t i = 0; i < m_stopPoints.size() - 1; ++i )
      {
         if ( z >= m_stopPoints[ i ] && z <= m_stopPoints[ i + 1 ] )
         {
            stopIndexLeft = i;
            break;
         }
      }

      size_t stopIndexRight = stopIndexLeft + 1;
      double x = ( z - m_stopPoints[ stopIndexLeft ] ) / ( m_stopPoints[ stopIndexRight ] - m_stopPoints[ stopIndexLeft ] );

      m_colourCache[ iColor ] = interpolate( m_stops[ stopIndexLeft ], m_stops[ stopIndexRight ], x ).rgba();
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getColour
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QColor Palette::getColour( double z ) const
{
   return QColor( getRgb( z ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getRgb
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QRgb Palette::getRgb( double z ) const
{
   z = z < 0 ? 0 : z;
   size_t index = z * s_numColoursInCache;

   index = index >= s_numColoursInCache ? s_numColoursInCache - 1 : index;

   return m_colourCache[ index ];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// interpolate
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QColor Palette::interpolate( const QColor& c1, const QColor& c2, double x ) const
{
   assert( x >= 0 && x <= 1 );

   double r1 = c1.redF();
   double r2 = c2.redF();
   double g1 = c1.greenF();
   double g2 = c2.greenF();
   double b1 = c1.blueF();
   double b2 = c2.blueF();

   double r = r1 + ( r2 - r1 ) * x;
   double g = g1 + ( g2 - g1 ) * x;
   double b = b1 + ( b2 - b1 ) * x;

   QColor result;
   result.setRgbF( r, g, b );
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// whiteBlackPalette
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Palette Palette::whiteBlackPalette()
{
   std::vector< double > stopPoints;
   std::vector< QColor > stops;

   stopPoints.push_back( 0 ); stops.push_back( QColor( Qt::white ) );
   stopPoints.push_back( 1 ); stops.push_back( QColor( Qt::black ) );

   return Palette( stops, stopPoints );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// blueRedPalette
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Palette Palette::blueRedPalette()
{
   std::vector< double > stopPoints;
   std::vector< QColor > stops;

   stopPoints.push_back( 0 ); stops.push_back( QColor( Qt::blue ) );
   stopPoints.push_back( 1 ); stops.push_back( QColor( Qt::red ) );

   return Palette( stops, stopPoints );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// heatPalette
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Palette Palette::heatPalette()
{
   std::vector< double > stopPoints;
   std::vector< QColor > stops;

   stopPoints.push_back( 0 ); stops.push_back( QColor( Qt::darkBlue ) );
   stopPoints.push_back( 1./6. ); stops.push_back( QColor( Qt::blue ) );
   stopPoints.push_back( 2./6. ); stops.push_back( QColor( Qt::cyan ) );
   stopPoints.push_back( 3./6. ); stops.push_back( QColor( Qt::green ) );
   stopPoints.push_back( 4./6. ); stops.push_back( QColor( Qt::yellow ) );
   stopPoints.push_back( 5./6. ); stops.push_back( QColor( Qt::red ) );
   stopPoints.push_back( 1 ); stops.push_back( QColor( Qt::darkRed ) );

   return Palette( stops, stopPoints );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// firePalette
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Palette Palette::firePalette()
{
   std::vector< double > stopPoints;
   std::vector< QColor > stops;

   stopPoints.push_back( 0 ); stops.push_back( QColor( Qt::darkRed ) );
   stopPoints.push_back( 1./3. ); stops.push_back( QColor( Qt::red ) );
   stopPoints.push_back( 2./3. ); stops.push_back( QColor( Qt::yellow ) );
   stopPoints.push_back( 1 ); stops.push_back( QColor( Qt::white ) );

   return Palette( stops, stopPoints );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// rainbowPalette
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Palette Palette::rainbowPalette()
{
   std::vector< double > stopPoints;
   std::vector< QColor > stops;

   stopPoints.push_back( 0 ); stops.push_back( QColor( Qt::red ) );
   stopPoints.push_back( 1./5. ); stops.push_back( QColor( Qt::yellow ) );
   stopPoints.push_back( 2./5. ); stops.push_back( QColor( Qt::green ) );
   stopPoints.push_back( 3./5. ); stops.push_back( QColor( Qt::cyan ) );
   stopPoints.push_back( 4./5. ); stops.push_back( QColor( Qt::blue ) );
   stopPoints.push_back( 1 ); stops.push_back( QColor( "violet" ) );

   return Palette( stops, stopPoints );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// inverseRainbowPalette
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Palette Palette::inverseRainbowPalette()
{
   std::vector< double > stopPoints;
   std::vector< QColor > stops;

   stopPoints.push_back( 0 ); stops.push_back( QColor( Qt::white ) );
   stopPoints.push_back( 1./5. ); stops.push_back( QColor( Qt::darkBlue ) );
   stopPoints.push_back( 2./5. ); stops.push_back( QColor( Qt::cyan ) );
   stopPoints.push_back( 3./5. ); stops.push_back( QColor( Qt::green ) );
   stopPoints.push_back( 4./5. ); stops.push_back( QColor( Qt::yellow ) );
   stopPoints.push_back( 1 ); stops.push_back( QColor( Qt::darkRed ) );

   return Palette( stops, stopPoints );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Static members
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const size_t Palette::s_numColoursInCache = 256;

} /// namespace Plotting
