#include "Regular2DHistogram.h"

#include <limits>

namespace Math
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Regular2DHistogram::Regular2DHistogram( size_t nBinsX, double xMin, double xMax, size_t nBinsY, double yMin, double yMax ) :
   m_contents( nBinsX, std::vector< double >( nBinsY, 0 ) ),
   m_missedContents( 0 ),
   m_xMin( xMin ),
   m_xMax( xMax ),
   m_xBinWidth( ( xMax - xMin ) / nBinsX ),
   m_nBinsX( nBinsX ),
   m_yMin( yMin ),
   m_yMax( yMax ),
   m_yBinWidth( ( yMax - yMin ) / nBinsY ),
   m_nBinsY( nBinsY )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setBinContent
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Regular2DHistogram::setBinContent( size_t iBinX, size_t iBinY, double value )
{
   m_contents[ iBinX ][ iBinY ] = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getBinContent
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Regular2DHistogram::getBinContent( size_t iBinX, size_t iBinY ) const
{
   return m_contents[ iBinX ][ iBinY ];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// add
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Regular2DHistogram::add( double x, double y, double weight )
{
   size_t binIndexX = getBinIndexX( x );
   size_t binIndexY = getBinIndexY( y );
   if ( binIndexX < m_nBinsX && binIndexY < m_nBinsY )
   {
      m_contents[ binIndexX ][ binIndexY ] += weight;
   }
   else
   {
      m_missedContents += weight;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getBinIndexX
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t Regular2DHistogram::getBinIndexX( double x ) const
{
   if ( x < m_xMin || x > m_xMax )
   {
      return m_nBinsX;
   }
   else
   {
      return ( x - m_xMin ) / m_xBinWidth;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getBinIndexY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t Regular2DHistogram::getBinIndexY( double y ) const
{
   if ( y < m_yMin || y > m_yMax )
   {
      return m_nBinsY;
   }
   else
   {
      return ( y - m_yMin ) / m_yBinWidth;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getNumBinsX
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t Regular2DHistogram::getNumBinsX() const
{
   return m_nBinsX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getNumBinsY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t Regular2DHistogram::getNumBinsY() const
{
   return m_nBinsY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getBinWidth
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Regular2DHistogram::getBinWidthX() const
{
   return m_xBinWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getBinWidthY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Regular2DHistogram::getBinWidthY() const
{
   return m_yBinWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMinX
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Regular2DHistogram::getMinX() const
{
   return m_xMin;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMaxX
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Regular2DHistogram::getMaxX() const
{
   return m_xMax;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMinY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Regular2DHistogram::getMinY() const
{
   return m_yMin;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMaxY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Regular2DHistogram::getMaxY() const
{
   return m_yMax;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMaximum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Regular2DHistogram::getMaximum() const
{
   double max = -std::numeric_limits< double >::max();
   for ( size_t iX = 0; iX < m_contents.size(); ++iX )
   {
      for ( size_t iY = 0; iY < m_contents.size(); ++iY )
      {
         if ( m_contents[ iX ][ iY ] > max )
         {
            max = m_contents[ iX ][ iY ];
         }
      }
   }
   return max;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMinimum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Regular2DHistogram::getMinimum() const
{
   double min = std::numeric_limits< double >::max();
   for ( size_t iX = 0; iX < m_contents.size(); ++iX )
   {
      for ( size_t iY = 0; iY < m_contents.size(); ++iY )
      {
         if ( m_contents[ iX ][ iY ] < min )
         {
            min = m_contents[ iX ][ iY ];
         }
      }
   }
   return min;
}

} /// namespace Math

