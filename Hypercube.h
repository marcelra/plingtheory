#ifndef HYPERCUBE_H
#define HYPERCUBE_H

#include "Interval.h"

#include <cstddef>
#include <vector>

namespace Math
{

/**
 * @class Hypercube
 * @brief Describes a hypercube.
 */
class Hypercube
{
   public:
      /**
       * Create a hypercube with bounds @param lowerBounds and @param upperBounds.
       */
      Hypercube( const std::vector< double >& lowerBounds, const std::vector< double >& upperBounds );
      /**
       * Create a hypercube by specifying a set of intervals @param intervals.
       */
      Hypercube( const std::vector< Interval >& intervals );

      /**
       * Get the dimensionality of the hypercube.
       */
      size_t getDimensionality() const;
      /**
       * Get the lower bound for index @param index.
       */
      double getMin( size_t index ) const;
      /**
       * Get the upper bound for index @param index.
       */
      double getMax( size_t index ) const;

   private:
      std::vector< double > m_lowerBounds;      //! Lower bounds of hypercube.
      std::vector< double > m_upperBounds;      //! Upper bounds of hypercube.
};

} /// namespace Math


#endif //HYPERCUBE_H
