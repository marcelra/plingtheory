#ifndef MOVINGAVERAGE_H
#define MOVINGAVERAGE_H

#include "RealVector.h"

namespace Math
{

class SampledMovingAverage
{
   public:
      SampledMovingAverage( size_t nSamples );
      SampledMovingAverage( const RealVector& weights );

      RealVector calculate( const RealVector& dataSet );

      static RealVector createGaussianWeights( size_t nSamples, double lambda );

   private:
      double getWeight( size_t iWeight );
      void normaliseWeights();

   private:
      RealVector     m_weights;
};

} /// namespace Math

#endif // MOVINGAVERAGE_H
