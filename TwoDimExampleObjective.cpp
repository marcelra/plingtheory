#include "TwoDimExampleObjective.h"

namespace Math
{

double TwoDimExampleObjective::evaluate( const RealVector& x ) const
{
   return x[0]*x[0] + x[1]*x[1];
}

size_t TwoDimExampleObjective::getNumParameters() const
{
   return 2;
}

} /// namespace Math
