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
      /**
       * @see createRange (integer variant). Works identically, but does not include the maxIndex.
       */
      static RealVector createRangeReal( size_t minIndex, size_t maxIndex );

      /**
       * Create a set @param nElements between @param min and @param max. @param min and @param max are both included.
       * The latter imposes the condition that nSteps >= 2.
       */
      static RealVector createRangeReal( double min, double max, size_t nSteps );

      /**
       * Converts a templated vector to a RealVector.
       */
      template< class T >
      static RealVector convertToRealVec( const std::vector< T >& vec );

      /**
       * Transpose matrix @param matrix.
       */
      template< class T >
      static std::vector< std::vector< T > > transpose( const std::vector< std::vector< T > >& matrix );

      /**
       * Converts a value vector to a pointer vector. The copy constructor of the objects in the vector should be well defined.
       */
      template< class T >
      static std::vector< std::unique_ptr< T > > convertToPointerVector( const std::vector< T >& input );

      /**
       * Cleanup vector.
       */
      template< class T >
      static void cleanupVector( std::vector< T* > vector );

      /**
       * Make a contiguous selection of a vector.
       */
      template< class T >
      static std::vector< T > selectContiguous( const std::vector< T >& vector, size_t firstIndex, size_t numElements );
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
   return getMaxValueAndIndex( valueVec, dummy );
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// transpose
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template< class T >
std::vector< std::vector< T > > Utils::transpose( const std::vector< std::vector< T > >& matrix )
{
   std::vector< std::vector< T > > result;

   if ( matrix.size() == 0 )
   {
      return result;
   }

   const size_t nRows = matrix.size();
   const size_t nCols = matrix[ 0 ].size();

   result.resize( nCols );
   for ( size_t iCol = 0; iCol < nCols; ++iCol )
   {
      result[ iCol ].resize( nRows );
   }

   for ( size_t iRow = 0; iRow < nRows; ++iRow )
   {
      assert( nCols == matrix[ iRow ].size() );

      for ( size_t iCol = 0; iCol < matrix[ iRow ].size(); ++iCol )
      {
         result[ iCol ][ iRow ] = matrix[ iRow ][ iCol ];
      }
   }

   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// convertToPointerVector
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template< class T >
std::vector< std::unique_ptr< T > > Utils::convertToPointerVector( const std::vector< T >& input )
{
   std::vector< std::unique_ptr< T > > result( input.size() );
   for ( size_t i = 0; i < input.size(); ++i )
   {
      result[ i ] = std::unique_ptr< T >( new T( input[ i ] ) );
   }
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// cleanupVector
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template< class T >
void Utils::cleanupVector( std::vector< T* > vector )
{
   for ( size_t i = 0; i < vector.size(); ++i )
   {
      delete vector[ i ];
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// selectContiguous
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template< class T >
std::vector< T > Utils::selectContiguous( const std::vector< T >& vector, size_t firstIndex, size_t numElements )
{
   std::vector< T > result( numElements );

   for ( size_t i = 0; i < numElements; ++i )
   {
      result[ i ] = vector[ i + firstIndex ];
   }
   return result;
}

#endif // UTILS_H
