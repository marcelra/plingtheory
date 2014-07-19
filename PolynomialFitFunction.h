#ifndef POLYNOMIALFITFUNCTION_H
#define POLYNOMIALFITFUNCTION_H

#include "FitFunctionBase.h"

namespace Math
{

/**
 * @class PolynomialFitFunction
 * @brief A polynomial function that can be used in a fitting context.
 */
class PolynomialFitFunction : public FitFunctionBase
{
   public:
      /**
       * Create a polynomial fit function of degree @param degree.
       */
      PolynomialFitFunction( size_t degree );

   public:
      /**
       * Defined in @see FitFunctionBase.
       */
      double operator()( double x ) const;
      /**
       * Defined in @see IRealFunction.
       */
      IRealFunction* clone() const;
};

} /// namespace Math

#endif // POLYNOMIALFITFUNCTION_H
