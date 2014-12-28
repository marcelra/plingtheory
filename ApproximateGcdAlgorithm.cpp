#include "ApproximateGcdAlgorithm.h"

#include "Logger.h"

#include <cassert>
#include <cmath>

namespace Math
{

ApproximateGcdAlgorithm::ApproximateGcdAlgorithm( double tolerance, const std::string& name, const AlgorithmBase* parent ) :
   AlgorithmBase( name, parent ),
   m_tolerance( tolerance )
{
   assert( tolerance >= 0 );
}

ApproximateGcdAlgorithm::Result ApproximateGcdAlgorithm::execute( double a, double b ) const
{
   getLogger() << Msg::Verbose << "execute: a = " << a << ", b = " << b << Msg::EndReq;
   if ( b <= m_tolerance )
   {
      Result result;
      result.gcd = a;
      result.remainder = b;
      return result;
   }
   else
   {
      return execute( b, fmod( a, b ) );
   }
}

ApproximateGcdAlgorithm::Result ApproximateGcdAlgorithm::execute( const std::vector< double >& vec ) const
{
   assert( vec.size() >= 2 );

   Result result = execute( vec[ 0 ], vec[ 1 ] );

   for ( size_t i = 2; i < vec.size(); ++i )
   {
      result = execute( result.gcd, vec[ i ] );
   }

   return result;
}


} /// namespace Math
