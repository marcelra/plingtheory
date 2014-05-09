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
      void setNumSamples( size_t numSamples );

      RealVectorEnsemble solve();

   private:
      RealVector proposeNew( const RealVector& x, double stepSize );
      bool accept( double probNew, double probOld );
      double calcProb( const RealVector& x );
      void updateStepSize();

      bool storeSolution( size_t iIter );

   private:
      const IObjectiveFunction&  m_objFunc;
      RealVectorEnsemble         m_mcmcChains;
      std::vector< double >      m_probOld;
      std::vector< double >      m_stepSizeVec;
      std::vector< size_t >      m_numAccepted;
      size_t                     m_numIterStepUpdate;
      size_t                     m_numIterations;
      size_t                     m_burnInSkip;
      size_t                     m_numSamples;
      double                     m_stepIncrease;
      double                     m_stepDecrease;
      double                     m_effLow;
      double                     m_effHigh;
      RandomNumberGenerator      m_random;
};

} /// namespace Math

#endif // MCMCOPTIMISER_H
