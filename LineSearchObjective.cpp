#include "LineSearchObjective.h"

#include <cmath>

namespace Math
{

LineSearchObjective::LineSearchObjective( const IObjectiveFunction& objFunc, const RealVector& currentParameters, const RealVector& currentGradient ) :
   m_objFunc( objFunc ),
   m_currentParameters( currentParameters ),
   m_currentGradient( currentGradient )
{}

double LineSearchObjective::evaluate( const RealVector& x ) const
{
   assert( x.size() == getNumParameters() );
   return m_objFunc.evaluate( m_currentParameters - fabs(x[0])*m_currentGradient );
}

size_t LineSearchObjective::getNumParameters() const
{
   return 1;
}

} /// namespace Math
