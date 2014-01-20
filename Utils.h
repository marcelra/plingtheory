#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cstddef>

/**
 * @class Utils
 * @brief Class with static utility methods
 */
class Utils
{
   public:
      /**
       * Get minimum value of a vector of doubles
       */
      template <class T>
      static double getMinValue( const std::vector< T >& valueVec );
      /**
       * Get maximum value of a vector of doubles
       */
      template <class T>
      static double getMaxValue( const std::vector< T >& valueVec );
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMinValue
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
double Utils::getMinValue( const std::vector< T >& valueVec )
{
   double result = 1e99;
   for ( size_t i = 0; i < valueVec.size(); ++i )
   {
      if ( valueVec[i] < result )
      {
         result = valueVec[i];
      }
   }
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMaxValue
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
double Utils::getMaxValue( const std::vector< T >& valueVec )
{
   double result = -1e99;
   for ( size_t i = 0; i < valueVec.size(); ++i )
   {
      if ( valueVec[i] > result )
      {
         result = valueVec[i];
      }
   }
   return result;
}

#endif // UTILS_H
