#include "McmcOptimiser.h"

#include "Logger.h"

namespace Math
{

McmcOptimiser::McmcOptimiser( const IObjectiveFunction& objFunc ) :
   m_objFunc( objFunc ),
   m_numIterations( 1000 ),
   m_burnInSkip( 200 ),
   m_stepMultiply( 1.0000001 ),
   m_random( 10 )
{
}

void McmcOptimiser::setStartValues( const RealVectorEnsemble& startValues )
{
   m_mcmcChains = startValues;
   m_probOld.resize( m_mcmcChains.size() );
   for ( size_t i = 0; i < m_mcmcChains.size(); ++i )
   {
      m_probOld[ i ] = calcProb( m_mcmcChains[ i ] );
   }
   setStepSize( 1 );
}

void McmcOptimiser::setStepSize( double stepWidth )
{
   m_stepSizeVec.assign( m_mcmcChains.size(), stepWidth );
}

void McmcOptimiser::setNumIterations( size_t numIterations )
{
   m_numIterations = numIterations;
}

RealVectorEnsemble McmcOptimiser::solve()
{
   RealVectorEnsemble solutions;
   for ( size_t iIter = 0; iIter < m_numIterations; ++iIter )
   {
      for ( size_t iChain = 0; iChain < m_mcmcChains.size(); ++iChain )
      {
         bool accepted = false;
         const RealVector& x = proposeNew( m_mcmcChains[ iChain ], m_stepSizeVec[ iChain ] );

         double probNew = calcProb( x );
         if ( accept( probNew, m_probOld[ iChain ] ) )
         {
            m_mcmcChains[ iChain ] = x;
            m_probOld[ iChain ] = probNew;
            accepted = true;
            m_stepSizeVec[ iChain ] /= m_stepMultiply;
         }
         else
         {
            m_stepSizeVec[ iChain ] *= m_stepMultiply;
         }
         if ( iIter > m_burnInSkip )
         {
            solutions.push_back( m_mcmcChains[ iChain ] );
         }
      }
   }
   return solutions;
}

RealVector McmcOptimiser::proposeNew( const RealVector& x, double stepSize )
{
   RealVector result( x );
   for ( size_t i = 0; i < x.size(); ++i )
   {
      result[ i ] += stepSize * m_random.uniform( -1, 1 ) * stepSize;
   }
   return result;
}

double McmcOptimiser::calcProb( const RealVector& x )
{
   return m_objFunc.evaluate( x );
}

bool McmcOptimiser::accept( double probNew, double probOld )
{
   if ( probNew > probOld )
   {
      return true;
   }
   else
   {
      bool doAccept = m_random.uniform( 0, probOld ) < probNew;
      return doAccept;
   }
}

void McmcOptimiser::setBurninSkip( size_t numIterations )
{
   m_burnInSkip = numIterations;
}

} /// namespace Math
