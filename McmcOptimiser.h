#ifndef MCMCOPTIMISER_H
#define MCMCOPTIMISER_H

#include "IObjectiveFunction.h"
#include "RandomNumberGenerator.h"
#include "RealVector.h"

namespace Math
{

typedef std::vector< RealVector > RealVectorEnsemble;

/**
 * @class McmcOptimiser
 * @brief Samples from probability distribution.
 */
class McmcOptimiser
{
   public:
      /**
       * Constructor, about to sample @param objFunc.
       */
      McmcOptimiser( const IObjectiveFunction& objFunc );

      /**
       * Set the start values. This also determines the number of Markov chains.
       */
      void setStartValues( const RealVectorEnsemble& startValues );
      /**
       * Set the initial step size for all chains.
       */
      void setStepSize( double stepSize );
      /**
       * Set the number of iterations.
       */
      void setNumIterations( size_t numIterations );
      /**
       * Skip @param numIterations iterations during burn-in period.
       */
      void setBurninSkip( size_t numIterations );
      /**
       * Set the number of samples to be sampled.
       */
      void setNumSamples( size_t numSamples );

      /**
       * Sample the PDF.
       */
      RealVectorEnsemble solve();

   private:
      /**
       * Perform one Markov step.
       */
      RealVector proposeNew( const RealVector& x, double stepSize );
      /**
       * Accept function.
       */
      bool accept( double probNew, double probOld );
      /**
       * Update the step size for every Markov chain.
       */
      void updateStepSize();

   private:
      const IObjectiveFunction&  m_objFunc;              //! Objective function.
      RealVectorEnsemble         m_mcmcChains;           //! The MCMC chains.
      std::vector< double >      m_probOld;              //! Previous probability
      std::vector< double >      m_stepSizeVec;          //! Step size per chain.
      std::vector< size_t >      m_numAccepted;          //! Number of accepted updates.
      size_t                     m_numIterStepUpdate;    //! Update step size after this number of iterations.
      size_t                     m_numIterations;        //! Total number of iterations.
      size_t                     m_burnInSkip;           //! Skip this number of iterations recording samples.
      size_t                     m_numSamples;           //! Total number of samples requested.
      double                     m_stepIncrease;         //! Increase step-size factor.
      double                     m_stepDecrease;         //! Decrease step-size factor.
      double                     m_effLow;               //! Lower bound proposal efficiency for decrease step-size.
      double                     m_effHigh;              //! Upper bound proposal efficiency for increase step-size.
      RandomNumberGenerator      m_random;               //! Random number generator.
};

} /// namespace Math

#endif // MCMCOPTIMISER_H
