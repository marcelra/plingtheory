#ifndef TWODIMEXAMPLEOBJECTIVE_H
#define TWODIMEXAMPLEOBJECTIVE_H

#include "IObjectiveFunction.h"

namespace Math
{

/**
 * @class TwoDimExampleObjective
 * @brief A tilted maximum-hat potential.
 * @see IObjectiveFunction.
 */
class TwoDimExampleObjective : public IObjectiveFunction
{
   public:
      size_t getNumParameters() const;
      double evaluate( const RealVector& x ) const;
};

} /// namespace Math

#endif // TWODIMEXAMPLEOBJECTIVE_H
