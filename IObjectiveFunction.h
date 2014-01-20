#ifndef IOBJECTIVEFUNCTION_H
#define IOBJECTIVEFUNCTION_H

#include "RealVector.h"

namespace Math
{

/**
 * @class IObjectiveFunction
 * @brief Abstract class/interface for an objective function.
 *
 * Approximated gradients are supplied via the calculateGradient method
 * Specialisations should override the pure virtual evaluate and getNumParameters methods
 */
class IObjectiveFunction
{
   public:
      /**
       * Destructor
       */
      virtual ~IObjectiveFunction();

   public:
      /**
       * Evaluate given an input vector @param x
       */
      virtual double evaluate( const RealVector& x ) const = 0;
      /**
       * Return the dimensionality of @param x
       */
      virtual size_t getNumParameters() const = 0;

   public:
      /**
       * Numeric approximation of gradient.
       */
      virtual RealVector calculateGradient( const RealVector& x, double delta = 1e-6 ) const;

};

} /// namespace Math

#endif // IOBJECTIVEFUNCTION_H
