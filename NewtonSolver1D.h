#ifndef NEWTONSOLVER1D_H
#define NEWTONSOLVER1D_H

#include "IRealFuncWithDerivative.h"
#include "RootFinder1DBase.h"

namespace Math
{

class NewtonSolver1D : public RootFinder1DBase
{
   public:
      NewtonSolver1D();
      Result solve( const IRealFuncWithDerivative& function, double value, double startX );

   private:
      static double                       s_failDerivThreshold;
};

} /// namespace Math

#endif // NEWTONSOLVER1D_H
