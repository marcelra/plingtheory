#include "RealVector.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// calcDerivative
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// isEqual
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool isEqual( const RealVector& x, const RealVector& y )
{
   if ( x.size() != y.size() )
   {
      return false;
   }
   for ( size_t i = 0; i < x.size(); ++i )
   {
      if ( x[ i ] != y [ i ] )
      {
         return false;
      }
   }
   return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// scale
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void scale( RealVector& x, double lambda )
{
   for ( size_t i = 0; i < x.size(); ++i )
   {
      x[ i ] *= lambda;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// normalise
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void normalise( RealVector& x )
{
   double norm = sum( x );
   scale( x, norm );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// sumElements
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double sumElements( const RealVector& x )
{
   double sum = 0;
   for ( size_t i = 0; i < x.size(); ++i )
   {
      sum += x[ i ];
   }
   return sum;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// calcMean
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double calcMean( const RealVector& x )
{
   return sumElements( x ) / x.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// realVector
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVector realVector( double x0 )
{
   RealVector result;
   result.push_back( x0 );
   return result;
}

RealVector realVector( double x0, double x1 )
{
   RealVector result;
   result.push_back( x0 );
   result.push_back( x1 );
   return result;
}

RealVector realVector( double x0, double x1, double x2 )
{
   RealVector result;
   result.push_back( x0 );
   result.push_back( x1 );
   result.push_back( x2 );
   return result;
}

RealVector realVector( double x0, double x1, double x2, double x3 )
{
   RealVector result;
   result.push_back( x0 );
   result.push_back( x1 );
   result.push_back( x2 );
   result.push_back( x3 );
   return result;
}

RealVector realVector( double x0, double x1, double x2, double x3, double x4 )
{
   RealVector result;
   result.push_back( x0 );
   result.push_back( x1 );
   result.push_back( x2 );
   result.push_back( x3 );
   result.push_back( x4 );
   return result;
}

RealVector realVector( double x0, double x1, double x2, double x3, double x4, double x5 )
{
   RealVector result;
   result.push_back( x0 );
   result.push_back( x1 );
   result.push_back( x2 );
   result.push_back( x3 );
   result.push_back( x4 );
   result.push_back( x5 );
   return result;
}

RealVector realVector( double x0, double x1, double x2, double x3, double x4, double x5, double x6 )
{
   RealVector result;
   result.push_back( x0 );
   result.push_back( x1 );
   result.push_back( x2 );
   result.push_back( x3 );
   result.push_back( x4 );
   result.push_back( x5 );
   result.push_back( x6 );
   return result;
}

RealVector realVector( double x0, double x1, double x2, double x3, double x4, double x5, double x6, double x7 )
{
   RealVector result;
   result.push_back( x0 );
   result.push_back( x1 );
   result.push_back( x2 );
   result.push_back( x3 );
   result.push_back( x4 );
   result.push_back( x5 );
   result.push_back( x6 );
   result.push_back( x7 );
   return result;
}

