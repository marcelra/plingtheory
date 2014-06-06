#ifndef IPDF_H
#define IPDF_H

#include <cstddef>
#include <memory>

#include "IRealFunction.h"

namespace Math
{

/**
 * @class IPdf
 * @brief Interface to probability density functions.
 */
class IPdf
{
   public:
      typedef std::unique_ptr< IPdf > Ptr;
      typedef std::unique_ptr< const IPdf > CPtr;

   public:
      /**
       * Virtual destructor for interface class.
       */
      virtual ~IPdf();

      /**
       * Evaluate density at point @param x. Returns zero outside bounds.
       */
      virtual double getDensity( double x ) const = 0;
      /**
       * Get probability that value is between @param x0 and @param x1.
       */
      virtual double getProbability( double x0, double x1 ) const = 0;
      /**
       * Get the integral from -inf to @param x.
       */
      virtual double getIntegral( double x ) const = 0;

      /**
       * Get minimum bound.
       */
      virtual double getMinX() const;
      /**
       * Get maximum bound.
       */
      virtual double getMaxX() const;

      /**
       * Get as function object.
       */
      IRealFunction::Ptr getDensityAsFunc() const;
};

} /// namespace Math

#endif // IPDF_H
