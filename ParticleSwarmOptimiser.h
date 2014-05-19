#ifndef PARTICLESWARMOPTIMISER_H
#define PARTICLESWARMOPTIMISER_H

#include "Interval.h"
#include "IObjectiveFunction.h"
#include "RandomNumberGenerator.h"
#include "RealVector.h"
#include "Hypercube.h"

#include <vector>

namespace Math
{

class ParticleSwarmOptimiser
{
   public:
      ParticleSwarmOptimiser( const IObjectiveFunction& objFunc, size_t numParticles, const Hypercube& solutionSpace );

      RealVector solve( size_t numIterations, std::vector< std::vector< RealVector > >* swarmTracker = 0 );
      void execIteration();

   private:
      void initialise();
      void updateParticlePositions();
      void updateBestValues();

   private:
      const IObjectiveFunction&  m_objFunc;
      double                     m_globalBest;
      std::vector< double >      m_particleBest;
      std::vector< RealVector >  m_particleBestPos;
      std::vector< RealVector >  m_particlePositions;
      std::vector< RealVector >  m_particleVelocities;
      RealVector                 m_bestSolution;
      Hypercube              m_solutionSpace;
      RandomNumberGenerator      m_randGen;

      double                     m_delta;
      double                     m_velocityPropagation;
      double                     m_localCoupling;
      double                     m_globalCoupling;
};

} /// namespace Math

#endif // PSOOPTIMISER_H
