#include "ParticleSwarmOptimiser.h"

#include <limits>

#include <iostream>

namespace
{
   double lowestObjFuncVal = -std::numeric_limits< double >::max();
}

namespace Math
{

SolutionSpace::SolutionSpace( const RealVector& lowerBounds, const RealVector& upperBounds ) :
   m_lowerBounds( lowerBounds ),
   m_upperBounds( upperBounds )
{}

size_t SolutionSpace::getDimensionality() const
{
   return m_lowerBounds.size();
}

double SolutionSpace::getMin( size_t index ) const
{
   return m_lowerBounds[ index ];
}

double SolutionSpace::getMax( size_t index ) const
{
   return m_upperBounds[ index ];
}

} /// namespace Math

namespace Math
{

ParticleSwarmOptimiser::ParticleSwarmOptimiser( const IObjectiveFunction& objFunc, size_t numParticles, const SolutionSpace& solutionSpace ) :
   m_objFunc( objFunc ),
   m_globalBest( lowestObjFuncVal ),
   m_particleBest( numParticles, lowestObjFuncVal ),
   m_particleBestPos( numParticles, RealVector( solutionSpace.getDimensionality() ) ),
   m_particlePositions( numParticles, RealVector( solutionSpace.getDimensionality() ) ),
   m_particleVelocities( numParticles, RealVector( solutionSpace.getDimensionality(), 1 ) ),
   m_solutionSpace( solutionSpace ),
   m_randGen( 1 ),
   m_delta( 0.1 ),
   m_velocityPropagation( 0.1 ),
   m_localCoupling( 0.4 ),
   m_globalCoupling( 0.1 )
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
         m_particlePositions[ iParticle ][ iDim ] = x;
      }
   }
   updateBestValues();
}

RealVector ParticleSwarmOptimiser::solve( size_t numIterations, std::vector< std::vector< RealVector > >* swarmTracker )
{
   for ( size_t iIter = 0; iIter < numIterations; ++iIter )
   {
      execIteration();
      std::cout << "Best objFuncVal = " << m_globalBest << std::endl;
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
         currentVel[ iPar ] += m_localCoupling * m_randGen.uniform() * ( currentPos[ iPar ] - localBest[ iPar ] );
         currentVel[ iPar ] += m_globalCoupling * m_randGen.uniform() * ( currentPos[ iPar ] - m_bestSolution[ iPar ] );
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
