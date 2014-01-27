#ifndef GRADDESCOPTIMISER_H
#define GRADDESCOPTIMISER_H

#include "RealVector.h"

#include "Logger.h"

/// Forward declares
namespace Math
{
   class IObjectiveFunction;
}

namespace Math
{

class GradDescOptimiser
{
   public:
      GradDescOptimiser( const IObjectiveFunction& func, const RealVector& startValues );
      virtual ~GradDescOptimiser();

      virtual RealVector solve();

      void setMaxIterations( size_t maxIterations );
      void setConvergenceCriterion( double objValChange );
      void setUseLineSearch( bool useLineSearch );

   private:
      const IObjectiveFunction&     m_func;
      RealVector                    m_input;
      size_t                        m_maxIterations;
      double                        m_objValChangeConvergence;
      bool                          m_useLineSearch;
};

} /// namespace Math

#endif // GRADDESCOPTIMISER_H
