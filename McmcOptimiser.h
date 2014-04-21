#ifndef MCMCOPTIMISER_H
#define MCMCOPTIMISER_H

#include "IObjectiveFunction.h"
#include "RandomNumberGenerator.h"
#include "RealVector.h"

namespace Math
{

typedef std::vector< RealVector > RealVectorEnsemble;

class McmcOptimiser
{
   public:
      McmcOptimiser( const IObjectiveFunction& objFunc );

      void setStartValues( const RealVectorEnsemble& startValues );
      void setStepSize( double stepSize );
      void setNumIterations( size_t numIterations );
      void setBurninSkip( size_t numIterations );

      RealVectorEnsemble solve();

   private:
      RealVector proposeNew( const RealVector& x, double stepSize );
      bool accept( double probNew, double probOld );
      double calcProb( const RealVector& x );

   private:
      const IObjectiveFunction&  m_objFunc;
      RealVectorEnsemble         m_mcmcChains;
      std::vector< double >      m_probOld;
      std::vector< double >      m_stepSizeVec;
      size_t                     m_numIterations;
      size_t                     m_burnInSkip;
      double                     m_stepMultiply;
      RandomNumberGenerator      m_random;
};

} /// namespace Math

#endif // MCMCOPTIMISER_H
