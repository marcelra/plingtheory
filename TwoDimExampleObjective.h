#ifndef TWODIMEXAMPLEOBJECTIVE_H
#define TWODIMEXAMPLEOBJECTIVE_H

#include "IObjectiveFunction.h"

namespace Math
{

class TwoDimExampleObjective : public IObjectiveFunction
{
   public:
      size_t getNumParameters() const;

      double evaluate( const RealVector& x ) const;

   public:
      static void test();
};

} /// namespace Math

#endif // TWODIMEXAMPLEOBJECTIVE_H
