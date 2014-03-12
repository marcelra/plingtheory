#ifndef IPDF_H
#define IPDF_H

#include <cstddef>

namespace Math
{

/**
 * @class IPdf
 * @brief Interface to probability density functions.
 */
class IPdf
{
   public:
      /**
       * Virtual destructor for interface class.
       */
      virtual ~IPdf();

      /**
       * Evaluate density at point @param x without checking if @param x is element of (x_min, x_max). In general it can
       * return garbage outside the defined region.
       */
      virtual double eval( double x ) const = 0;
      /**
       * Evaluate density at point @param x. Returns zero outside bounds.
       */
      virtual double getDensity( double x ) const = 0;
      /**
       * Get probability that value is between @param x0 and @param x1.
       */
      virtual double getProbability( double x0, double x1 ) const = 0;

      /**
       * Get minimum bound.
       */
      virtual double getMinX() const = 0;
      /**
       * Get maximum bound.
       */
      virtual double getMaxX() const = 0;
};

} /// namespace Math

#endif // IPDF_H
