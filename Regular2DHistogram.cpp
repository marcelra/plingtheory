#include "Regular2DHistogram.h"

namespace Math
{

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

void Regular2DHistogram::setBinContent( size_t iBinX, size_t iBinY, double value )
{
   m_contents[ iBinX ][ iBinY ] = value;
}

double Regular2DHistogram::getBinContent( size_t iBinX, size_t iBinY ) const
{
   return m_contents[ iBinX ][ iBinY ];
}

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

size_t Regular2DHistogram::getNumBinsX() const
{
   return m_nBinsX;
}

size_t Regular2DHistogram::getNumBinsY() const
{
   return m_nBinsY;
}

double Regular2DHistogram::getBinWidthX() const
{
   return m_xBinWidth;
}

double Regular2DHistogram::getBinWidthY() const
{
   return m_yBinWidth;
}

} /// namespace Math

