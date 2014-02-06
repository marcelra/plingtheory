#include "RealVector.h"

RealVector calcDerivative( const RealVector& x )
{
   RealVector result( x.size() );
   for ( size_t i = 1; i < x.size() - 1; ++i )
   {
      result[i] = ( x[i+1] - x[i-1] ) / 2;
   }
   result[0] = x[1] - x[0];
   result[ x.size() - 1 ] = x[ x.size() - 1 ] - x[ x.size() - 2 ];
   return result;
}
