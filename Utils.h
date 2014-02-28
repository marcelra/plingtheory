#ifndef UTILS_H
#define UTILS_H

#include <cstddef>
#include <limits>
#include <vector>

/**
 * @class Utils
 * @brief Class with static utility methods
 */
class Utils
{
   public:
      /**
       * Get minimum value of a vector
       */
      template <class T>
      static double getMinValue( const std::vector< T >& valueVec );
      /**
       * Get maximum value of a vector
       */
      template <class T>
      static double getMaxValue( const std::vector< T >& valueVec );
      /**
       * Get minimum value and position
       */
      template <class T>
      static double getMinValueAndIndex( const std::vector< T >& valueVec, size_t& index );
      /**
       * Get maximum value and position
       */
      template <class T>
      static double getMaxValueAndIndex( const std::vector< T >& valueVec, size_t& index );
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMinValue
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
double Utils::getMinValue( const std::vector< T >& valueVec )
{
   size_t dummy;
   return getMinValueAndIndex( valueVec, dummy );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMinValue
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
double Utils::getMaxValue( const std::vector< T >& valueVec )
{
   size_t dummy;
   return getMinValueAndIndex( valueVec, dummy );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMinValueAndIndex
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
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
template <class T>
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

#endif // UTILS_H
