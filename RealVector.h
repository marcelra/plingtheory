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
typedef std::unique_ptr< RealVector > RealVectorPtr;

/**
 * Operator overloading for RealVector.
 */
RealVector operator+( const RealVector& x, const RealVector& y );
RealVector operator-( const RealVector& x, const RealVector& y );
RealVector operator*( const RealVector& x, double lambda );
RealVector operator*( double lambda, const RealVector& x );
RealVector operator/( const RealVector& x, double lambda );
double operator*( const RealVector& x, const RealVector& y );

RealVector& operator*=( RealVector& x, double lambda );
RealVector& operator/=( RealVector& x, double lambda );

RealVector operator+( const RealVector& x, double a );
RealVector operator+( double a, const RealVector& x );
RealVector operator-( const RealVector& x, double a );
RealVector operator-( double a, const RealVector& x );

/**
 * Modify vector.
 */
void scale( RealVector& x, double lambda );
void normalise( RealVector& x );

/**
 * Create.
 */
RealVector realVector( double x0 );
RealVector realVector( double x0, double x1 );
RealVector realVector( double x0, double x1, double x2 );
RealVector realVector( double x0, double x1, double x2, double x3 );
RealVector realVector( double x0, double x1, double x2, double x3, double x4 );
RealVector realVector( double x0, double x1, double x2, double x3, double x4, double x5 );
RealVector realVector( double x0, double x1, double x2, double x3, double x4, double x5, double x6 );
RealVector realVector( double x0, double x1, double x2, double x3, double x4, double x5, double x6, double x7 );

/**
 * Miscellaneous.
 */
RealVector calcDerivative( const RealVector& x );
bool isEqual( const RealVector& x, const RealVector& y );
double sumElements( const RealVector& x );
double calcMean( const RealVector& x );

/** Point wise multiplication */
RealVector pwm( const RealVector& x, const RealVector& y );

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

inline RealVector operator+( const RealVector& x, double a )
{
   RealVector result( x );
   for ( size_t i = 0; i < x.size(); ++i )
   {
      result[ i ] += a;
   }
   return result;
}

inline RealVector operator+( double a, const RealVector& x )
{
   return x + a;
}

inline RealVector operator-( const RealVector& x, double a )
{
   RealVector result( x );
   for ( size_t i = 0; i < x.size(); ++i )
   {
      result[ i ] -= a;
   }
   return result;
}

inline RealVector operator-( double a, const RealVector& x )
{
   return x - a;
}

inline double sum( const RealVector& x )
{
   double result = 0;
   for ( size_t i = 0; i < x.size(); ++i )
   {
      result += x[ i ];
   }
   return result;
}

inline RealVector pwm( const RealVector& x, const RealVector& y )
{
   RealVector result( x );
   for ( size_t i = 0; i < x.size(); ++i )
   {
      result[ i ] *= y[ i ];
   }
   return result;
}

inline RealVector& operator*=( RealVector& x, double lambda )
{
   for ( size_t i = 0; i < x.size(); ++i )
   {
      x[ i ] *= lambda;
   }
   return x;
}

inline RealVector& operator/=( RealVector& x, double lambda )
{
   for ( size_t i = 0; i < x.size(); ++i )
   {
      x[ i ] /= lambda;
   }
   return x;
}

#endif // REALVECTOR_H
