#ifndef UTILS_H
#define UTILS_H

#include "RealVector.h"

#include <cstddef>
#include <limits>
#include <vector>

typedef std::vector< size_t > IndexVector;

/**
 * @class Utils
 * @brief Class with static utility methods.
 */
class Utils
{
   public:
      /**
       * Get minimum value of a vector.
       */
      template< class T >
      static double getMinValue( const std::vector< T >& valueVec );
      /**
       * Get maximum value of a vector.
       */
      template< class T >
      static double getMaxValue( const std::vector< T >& valueVec );
      /**
       * Get minimum value and position.
       */
      template< class T >
      static double getMinValueAndIndex( const std::vector< T >& valueVec, size_t& index );
      /**
       * Get maximum value and position.
       */
      template< class T >
      static double getMaxValueAndIndex( const std::vector< T >& valueVec, size_t& index );

      /**
       * Find minima in @param vector.
       * Flat regions minima (i.e. a number of samples with identical values that are
       * will yield two minima, one at the start of the flat region and one at the end.
       */
      static std::vector< size_t > findMinima( const RealVector& vector );
      /**
       * Get selected indices, @param selectedIndices, from a vector @param vector.
       */
      static RealVector createSelection( const RealVector& vector, const IndexVector& selectedIndices );

      /**
       * Create an index vector of consecutive indices between @param minIndex and @param maxIndex. Index @param maxIndex
       * is not included in the vector.
       */
      static IndexVector createRange( size_t minIndex, size_t maxIndex );
      static RealVector createRangeReal( size_t minIndex, size_t maxIndex );

      template< class T >
      static RealVector convertToRealVec( const std::vector< T >& vec );
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMinValue
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template< class T >
double Utils::getMinValue( const std::vector< T >& valueVec )
{
   size_t dummy;
   return getMinValueAndIndex( valueVec, dummy );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMinValue
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template< class T >
double Utils::getMaxValue( const std::vector< T >& valueVec )
{
   size_t dummy;
   return getMinValueAndIndex( valueVec, dummy );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMinValueAndIndex
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template< class T >
double Utils::getMinValueAndIndex( const std::vector< T >& valueVec, size_t& index )
{
   double result = std::numeric_limits< double >::max();
   for ( size_t i = 0; i < valueVec.size(); ++i )
   {
      if ( valueVec[i] < result )
      {
         index = i;
         result = valueVec[i];
      }
   }
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMaxValueAndIndex
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template< class T >
double Utils::getMaxValueAndIndex( const std::vector< T >& valueVec, size_t& index )
{
   double result = -std::numeric_limits< double >::max();
   for ( size_t i = 0; i < valueVec.size(); ++i )
   {
      if ( valueVec[i] > result )
      {
         index = i;
         result = valueVec[i];
      }
   }
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// convertToRealVec
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template< class T >
RealVector Utils::convertToRealVec( const std::vector< T >& vec )
{
   RealVector result( vec.size() );
   for ( size_t i = 0; i < result.size(); ++ i )
   {
      result[ i ] = static_cast< double >( vec[ i ] );
   }
   return result;
}

#endif // UTILS_H
