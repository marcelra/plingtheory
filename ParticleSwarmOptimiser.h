#ifndef PARTICLESWARMOPTIMISER_H
#define PARTICLESWARMOPTIMISER_H

#include "IObjectiveFunction.h"
#include "RandomNumberGenerator.h"
#include "RealVector.h"

#include <vector>

namespace Math
{

class SolutionSpace
{
   public:
      SolutionSpace( const RealVector& lowerBounds, const RealVector& upperBounds );

      size_t getDimensionality() const;
      double getMin( size_t index ) const;
      double getMax( size_t index ) const;

   private:
      RealVector     m_lowerBounds;
      RealVector     m_upperBounds;
};

class ParticleSwarmOptimiser
{
   public:
      ParticleSwarmOptimiser( const IObjectiveFunction& objFunc, size_t numParticles, const SolutionSpace& solutionSpace );

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
      SolutionSpace              m_solutionSpace;
      RandomNumberGenerator      m_randGen;

      double                     m_delta;
      double                     m_velocityPropagation;
      double                     m_localCoupling;
      double                     m_globalCoupling;
};

} /// namespace Math

#endif // PSOOPTIMISER_H
