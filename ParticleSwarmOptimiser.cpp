#include "ParticleSwarmOptimiser.h"

#include <limits>

namespace
{
   double lowestObjFuncVal = -std::numeric_limits< double >::max();
}

namespace Math
{

ParticleSwarmOptimiser::ParticleSwarmOptimiser( const IObjectiveFunction& objFunc, size_t numParticles, const Hypercube& solutionSpace ) :
   m_objFunc( objFunc ),
   m_globalBest( lowestObjFuncVal ),
   m_particleBest( numParticles, lowestObjFuncVal ),
   m_particleBestPos( numParticles, RealVector( solutionSpace.getDimensionality() ) ),
   m_particlePositions( numParticles, RealVector( solutionSpace.getDimensionality() ) ),
   m_particleVelocities( numParticles, RealVector( solutionSpace.getDimensionality(), 1 ) ),
   m_solutionSpace( solutionSpace ),
   m_randGen( 1 ),
   m_delta( 1 ),
   m_velocityPropagation( 0.5 ),
   m_localCoupling( -0.5 ),
   m_globalCoupling( 1 )
{
   initialise();
}

void ParticleSwarmOptimiser::initialise()
{
   for ( size_t iParticle = 0; iParticle < m_particleBest.size(); ++iParticle )
   {
      for ( size_t iDim = 0; iDim < m_solutionSpace.getDimensionality(); ++iDim )
      {
         double x = m_randGen.uniform( m_solutionSpace.getMin( iDim ), m_solutionSpace.getMax( iDim ) );
         double v = m_randGen.uniform();
         m_particlePositions[ iParticle ][ iDim ] = x;
         m_particleVelocities[ iParticle ][ iDim ] = v;
      }
   }
   updateBestValues();
}

RealVector ParticleSwarmOptimiser::solve( size_t numIterations, std::vector< std::vector< RealVector > >* swarmTracker )
{
   for ( size_t iIter = 0; iIter < numIterations; ++iIter )
   {
      execIteration();
      if ( swarmTracker )
      {
         swarmTracker->push_back( m_particlePositions );
      }
   }
   return m_bestSolution;
}

void ParticleSwarmOptimiser::execIteration()
{
   updateParticlePositions();
   updateBestValues();
}

void ParticleSwarmOptimiser::updateParticlePositions()
{
   for ( size_t iParticle = 0; iParticle < m_particlePositions.size(); ++iParticle )
   {
      RealVector& currentPos = m_particlePositions[ iParticle ];
      RealVector& currentVel = m_particleVelocities[ iParticle ];
      RealVector& localBest = m_particleBestPos[ iParticle ];
      for ( size_t iPar = 0; iPar < m_objFunc.getNumParameters(); ++iPar )
      {
         currentVel[ iPar ] *= m_velocityPropagation;
         currentVel[ iPar ] += m_localCoupling * m_randGen.uniform() * ( - currentPos[ iPar ] + localBest[ iPar ] );
         currentVel[ iPar ] += m_globalCoupling * m_randGen.uniform() * ( - currentPos[ iPar ] + m_bestSolution[ iPar ] );
         currentPos[ iPar ] += m_delta * currentVel[ iPar ];
      }
   }
}

void ParticleSwarmOptimiser::updateBestValues()
{
   size_t bestIndex = m_particlePositions.size();
   for ( size_t iParticle = 0; iParticle < m_particlePositions.size(); ++iParticle )
   {
      double objFuncVal = m_objFunc.evaluate( m_particlePositions[ iParticle ] );
      if ( objFuncVal > m_particleBest[ iParticle ] )
      {
         m_particleBest[ iParticle ] = objFuncVal;
         m_particleBestPos[ iParticle ] = m_particlePositions[ iParticle ];
      }
      if ( objFuncVal > m_globalBest )
      {
         bestIndex = iParticle;
      }
   }
   if ( bestIndex != m_particlePositions.size() )
   {
      m_bestSolution = m_particlePositions[ bestIndex ];
      m_globalBest = m_particleBest[ bestIndex ];
   }
}



} /// namespace Math
