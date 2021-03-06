#include "LinearInterpolator.h"

#include "SortCache.h"

namespace Math
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LinearInterpolator::LinearInterpolator( const RealVector& x, const RealVector& y )
{
   assert( x.size() > 0 );
   assert( x.size() == y.size() );

   SortCache sc( x );
   m_x = sc.applyTo( x );
   m_y = sc.applyTo( y );

   m_xWidth = m_x.back() - m_x.front();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// operator()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double LinearInterpolator::operator()( double x ) const
{
   if ( x <= m_x.front() )
   {
      return m_y.front();
   }
   if ( x >= m_x.back() )
   {
      return m_y.back();
   }

   size_t iInterval = findInterval( x );
   return interpolate( x, m_x[ iInterval ], m_x[ iInterval + 1 ], m_y[ iInterval ], m_y[ iInterval + 1 ] );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// findInterval
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t LinearInterpolator::findInterval( double x ) const
{
   size_t iHypo = ( x - m_x.front() ) / m_xWidth * m_x.size();
   iHypo = iHypo >= m_x.size() - 1 ? m_x.size() - 2 : iHypo;

   RealVector::const_iterator it = m_x.begin();
   std::advance( it, iHypo );

   int searchDirection = 1;

   if ( *it > x )
   {
      searchDirection = -1;
   }

   int intervalIndex = iHypo;

   while ( true )
   {
      double xLow = *( it );
      double xHigh = *( it + 1 );

      if ( xLow <= x && x < xHigh )
      {
         assert( intervalIndex >= 0 && intervalIndex < static_cast< int >( m_x.size() ) - 1 );
         break;
      }

      std::advance( it, searchDirection );
      intervalIndex += searchDirection;
   }

   return intervalIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// clone
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IRealFunction* LinearInterpolator::clone() const
{
   return new LinearInterpolator( *this );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// static methods LinearInterpolator
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// interpolate
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double LinearInterpolator::interpolate( double x, double x0, double x1, double y0, double y1 )
{
   assert( x0 <= x && x <= x1 );

   double dX = x1 - x0;
   if ( dX < 1e-16 )
   {
      return y0;
   }
   double dY = y1 - y0;
   return y0 + dY / dX * ( x - x0 );
}

} /// namespace Math
