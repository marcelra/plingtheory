#ifndef IREALFUNCWITHDERIVATIVE_H
#define IREALFUNCWITHDERIVATIVE_H

#include "IRealFunction.h"

namespace Math
{

class IRealFuncWithDerivative : public IRealFunction
{
   public:
      virtual ~IRealFuncWithDerivative();

      virtual double deriv( double x ) const = 0;

   public:
      RealVector evalDerivMany( const RealVector& xValues ) const;
};

} /// namespace Math

#endif // IREALFUNCWITHDERIVATIVE_H
