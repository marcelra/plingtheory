#include "Palette.h"

#include <cassert>

namespace Plotting
{

Palette::Palette( const std::vector< QColor >& stops, const std::vector< double >& stopPoints ) :
   m_stops( stops ),
   m_stopPoints( stopPoints )
{
   assert( stopPoints[0] == 0 );
   assert( stopPoints[ stopPoints.size() - 1 ] == 1 );
   assert( stopPoints.size() == m_stops.size() );
}

QColor Palette::getColour( double z ) const
{
   assert( z >= 0 && z <= 1 );

   size_t stopIndexLeft;
   for ( size_t i = 0; i < m_stopPoints.size() - 1; ++i )
   {
      if ( z > m_stopPoints[ i ] && z < m_stopPoints[ i + 1] )
      {
         stopIndexLeft = i;
         break;
      }
   }

   size_t stopIndexRight = stopIndexLeft + 1;
   double x = ( z - m_stopPoints[ stopIndexLeft ] ) / ( m_stopPoints[ stopIndexRight ] - m_stopPoints[ stopIndexLeft ] );

   return interpolate( m_stops[ stopIndexLeft ], m_stops[ stopIndexRight ], x );
}

QColor Palette::interpolate( const QColor& c1, const QColor& c2, double x ) const
{
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

Palette Palette::whiteBlackPalette()
{
   std::vector< double > stopPoints;
   std::vector< QColor > stops;

   stopPoints.push_back( 0 ); stops.push_back( QColor( Qt::white ) );
   stopPoints.push_back( 1 ); stops.push_back( QColor( Qt::black ) );

   return Palette( stops, stopPoints );
}

Palette Palette::blueRedPalette()
{
   std::vector< double > stopPoints;
   std::vector< QColor > stops;

   stopPoints.push_back( 0 ); stops.push_back( QColor( Qt::blue ) );
   stopPoints.push_back( 1 ); stops.push_back( QColor( Qt::red ) );

   return Palette( stops, stopPoints );
}

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

} /// namespace Plotting
