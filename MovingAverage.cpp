#include "MovingAverage.h"

#include <cassert>
#include <cmath>

namespace Math
{

SampledMovingAverage::SampledMovingAverage( size_t nSamples ) :
   m_weights( nSamples, 1. / nSamples )
{
   assert( ( nSamples % 2 ) == 1 );
}

SampledMovingAverage::SampledMovingAverage( const RealVector& weights ) :
   m_weights( weights )
{
   normaliseWeights();
   assert( ( m_weights.size() % 2 ) == 1 );
}

RealVector SampledMovingAverage::calculate( const RealVector& dataSet )
{
   RealVector result( dataSet.size() );

   size_t nSamplesOneSide = ( m_weights.size() - 1 ) / 2;

   for ( size_t iSample = nSamplesOneSide; iSample < dataSet.size() - nSamplesOneSide; ++iSample )
   {
      double avg = 0;
      for ( size_t iMovAvg = 0; iMovAvg < m_weights.size(); ++iMovAvg )
      {
         avg += dataSet[ iMovAvg + iSample - nSamplesOneSide ] * m_weights[ iMovAvg ];
      }
      result[ iSample ] = avg;
   }

   for ( size_t iSample = 0; iSample < nSamplesOneSide; ++iSample )
   {
      result[ iSample ] = result[ nSamplesOneSide ];
      result[ result.size() - 1 - iSample ] = result[ result.size() - 1 - nSamplesOneSide ];
   }

   return result;
}

double SampledMovingAverage::getWeight( size_t iWeight )
{
   return m_weights[iWeight];
}

void SampledMovingAverage::normaliseWeights()
{
   double norm = 0;
   for ( size_t i = 0; i < m_weights.size(); ++i )
   {
      norm += m_weights[i];
   }

   for ( size_t i = 0; i <  m_weights.size(); ++i )
   {
      m_weights[i] /= norm;
   }
}

RealVector SampledMovingAverage::createGaussianWeights( size_t nSamples, double lambda )
{
   assert( ( nSamples % 2 ) == 1 );
   int nSamplesPerSide = ( nSamples - 1 ) / 2;

   RealVector result( nSamples );
   size_t j = 0;
   for ( int i = -nSamplesPerSide; i < nSamplesPerSide; ++i, ++j )
   {
      result[j] = exp( - i*i / lambda );
   }
   return result;
}

} /// namespace Math
