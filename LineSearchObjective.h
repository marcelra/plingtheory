#ifndef LINESEARCHOBJECTIVE_H
#define LINESEARCHOBJECTIVE_H

#include "IObjectiveFunction.h"

namespace Math
{

class LineSearchObjective : public IObjectiveFunction
{
   public:
      LineSearchObjective( const IObjectiveFunction& objFunc, const RealVector& currentParameters, const RealVector& currentGradient );

      double evaluate( const RealVector& gamma ) const;
      size_t getNumParameters() const;

   private:
      const IObjectiveFunction&    m_objFunc;
      const RealVector&            m_currentParameters;
      const RealVector&            m_currentGradient;
};

} /// namespace Math

#endif // LINESEARCHOBJECTIVE_H
