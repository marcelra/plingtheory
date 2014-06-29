#include "Palette.h"

#include <QDebug>

#include <cassert>

namespace Plotting
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Palette::Palette( const std::vector< QColor >& stops, const std::vector< double >& stopPoints ) :
   m_stops( stops ),
   m_stopPoints( stopPoints )
{
   assert( stopPoints[0] == 0 );
   assert( stopPoints[ stopPoints.size() - 1 ] == 1 );
   assert( stopPoints.size() == m_stops.size() );
   assert( stopPoints.size() >= 2 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getColour
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QColor Palette::getColour( double z ) const
{
   // assert( z >= 0 && z <= 1 );
   z = z > 1 ? 0.999 : z;

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

   return interpolate( m_stops[ stopIndexLeft ], m_stops[ stopIndexRight ], x );
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
   result.setRedF( r );
   result.setGreenF( g );
   result.setBlueF( b );
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

} /// namespace Plotting
