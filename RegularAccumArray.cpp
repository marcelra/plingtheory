#include "RegularAccumArray.h"

namespace Math
{

RegularAccumArray::RegularAccumArray( size_t nBins, double minX, double maxX ) :
   m_underflow( -1e99, minX ),
   m_overflow( maxX, 1e99 ),
   m_binWidth( ( maxX - minX ) / nBins )
{
   m_bins.reserve( nBins );
   double binXleft = minX;
   double binXright = minX + m_binWidth;
   for ( size_t i = 0; i < nBins; ++i )
   {
      m_bins.push_back( Bin( binXleft, binXright ) );
      binXleft = binXright;
      binXright += m_binWidth;
   }
}

RegularAccumArray::~RegularAccumArray()
{}

double RegularAccumArray::getBinContent( size_t iBin ) const
{
   return m_bins[ iBin ].getContents();
}

double RegularAccumArray::getMissedContent() const
{
   return 0;
}

double RegularAccumArray::getUnderflow() const
{
   return m_underflow.getContents();
}

double RegularAccumArray::getOverflow() const
{
   return m_overflow.getContents();
}

const TwoTuple& RegularAccumArray::getBinEntries( size_t iBin ) const
{
   return m_bins[ iBin ].getEntries();
}

const std::vector< IAccumArray::Bin >& RegularAccumArray::getBins() const
{
   return m_bins;
}

size_t RegularAccumArray::getNumBins() const
{
   return m_bins.size();
}

double RegularAccumArray::getMinX() const
{
   return m_underflow.getMaxX();
}

double RegularAccumArray::getMaxX() const
{
   return m_overflow.getMinX();
}

size_t RegularAccumArray::findBin( double x ) const
{
   double minX = m_underflow.getMaxX();
   double indexD = ( x - minX ) / m_binWidth;
   size_t index = indexD;
   return index;
}

IAccumArray::Bin RegularAccumArray::getBin( size_t binIndex ) const
{
   return m_bins[ binIndex ];
}

void RegularAccumArray::add( double x, double value )
{
   if ( x < m_underflow.getMaxX() )
   {
      m_underflow.add( x, value );
   }
   else if ( x > m_overflow.getMinX() )
   {
      m_overflow.add( x, value );
   }
   else
   {
      size_t binIndex = findBin( x );
      m_bins[ binIndex ].add( x, value );
   }
}

} /// namespace Math

