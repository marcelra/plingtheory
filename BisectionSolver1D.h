#ifndef BISECTIONSOLVER1D_H
#define BISECTIONSOLVER1D_H

#include "Interval.h"
#include "IRealFunction.h"
#include "RootFinder1DBase.h"

namespace Math
{

class BisectionSolver1D : public RootFinder1DBase
{
   public:
      BisectionSolver1D();
      Result solve( const IRealFunction& function, double value, const Interval& interval );
};

} /// namespace Math

#endif // BISECTIONSOLVER1D_H
