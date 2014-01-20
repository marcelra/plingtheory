#ifndef REALVECTOR_H
#define REALVECTOR_H

#include <cassert>
#include <cstddef>
#include <memory>
#include <vector>

/**
 * RealVector typedef
 */
typedef std::vector< double > RealVector;
typedef std::auto_ptr< RealVector > RealVectorPtr;

/**
 * Operator overloading for RealVector
 */
RealVector operator+( const RealVector& x, const RealVector& y );
RealVector operator-( const RealVector& x, const RealVector& y );
RealVector operator*( const RealVector& x, double lambda );
RealVector operator*( double lambda, const RealVector& x );
RealVector operator/( const RealVector& x, double lambda );
double operator*( const RealVector& x, const RealVector& y );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// inline methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline RealVector operator+( const RealVector& x, const RealVector& y )
{
   assert( x.size() == y.size() );
   RealVector result( x );
   for ( size_t i = 0; i < x.size(); ++i )
   {
      result[i] += y[i];
   }
   return result;
}

inline RealVector operator-( const RealVector& x, const RealVector& y )
{
   assert( x.size() == y.size() );
   RealVector result( x );
   for ( size_t i = 0; i < x.size(); ++i )
   {
      result[i] -= y[i];
   }
   return result;
}

inline RealVector operator*( const RealVector& x, double lambda )
{
   RealVector result( x );
   for ( size_t i = 0; i < x.size(); ++i )
   {
      result[i] *= lambda;
   }
   return result;
}

inline RealVector operator*( double lambda, const RealVector& x )
{
   return x*lambda;
}

inline RealVector operator/( const RealVector& x, double lambda )
{
   RealVector result( x );
   for ( size_t i = 0; i < x.size(); ++i )
   {
      result[i] /= lambda;
   }
   return result;
}

inline double operator*( const RealVector& x, const RealVector& y )
{
   double result = 0;
   assert( x.size() == y.size() );
   for ( size_t i = 0; i < x.size(); ++i )
   {
      result += x[i]*y[i];
   }
   return result;
}

#endif // REALVECTOR_H
