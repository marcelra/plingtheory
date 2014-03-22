#include "Interval.h"

#include <cassert>

Interval::Interval( double min, double max ) :
   m_min( min ),
   m_max( max )
{
   assert( m_min <= m_max );
}

Interval Interval::createFromValues( double value1, double value2 )
{
   if ( value2 <= value1 )
   {
      return Interval( value2, value1 );
   }
   else
   {
      return Interval( value1, value2 );
   }
}

bool Interval::contains( double x ) const
{
   if ( x >= m_min && x <= m_max )
   {
      return true;
   }
   else
   {
      return false;
   }
}
