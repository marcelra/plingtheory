#ifndef GRADDESCOPTIMISER_H
#define GRADDESCOPTIMISER_H

#include "AlgorithmBase.h"
#include "RealVector.h"

/// Forward declares
namespace Math
{
   class IObjectiveFunction;
}

namespace Math
{

class GradDescOptimiser : public AlgorithmBase
{
   public:
      GradDescOptimiser( const IObjectiveFunction& func, const RealVector& startValues, const std::string& algorithmName = "GradDescOptimiser", const AlgorithmBase* parent = 0 );
      virtual ~GradDescOptimiser();

      virtual RealVector solve();

      void setMaxIterations( size_t maxIterations );
      void setConvergenceCriterion( double objValChange );
      void setGamma( double gamma );

   private:
      const IObjectiveFunction&     m_func;
      double                        m_gamma;
      RealVector                    m_input;
      size_t                        m_maxIterations;
      double                        m_objValChangeConvergence;
};

} /// namespace Math

#endif // GRADDESCOPTIMISER_H
