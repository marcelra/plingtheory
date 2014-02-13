#ifndef MOVINGAVERAGE_H
#define MOVINGAVERAGE_H

#include "RealVector.h"

#include "TwoTuple.h"

namespace Math
{

/**
 * @class SampledMovingAverage
 * @brief Helper class to calculate moving averages for data set that are equidistant (sampled)
 */
class SampledMovingAverage
{
   public:
      /**
       * Instantiate a moving average calculator. @param nSamples is the total number of samples that are used in the
       * calculation. It is required that nSamples is odd so that the center sample is unambiguous.
       */
      SampledMovingAverage( size_t nSamples );
      /**
       * Instantiate a moving average calculator with given weights. @see comment at constructor above. Also here the number of samples should be odd.
       */
      SampledMovingAverage( const RealVector& weights );

      /**
       * Calculate moving average for @param dataSet
       */
      RealVector calculate( const RealVector& dataSet ) const;

      /**
       * Helper function that creates Gaussian weights. @param nSamples should be odd and the centre of the Gaussian is located at the middle sample.
       */
      static RealVector createGaussianWeights( size_t nSamples, double sigma );

   private:
      /**
       * Helper method to retreive weights
       */
      double getWeight( size_t iWeight ) const;
      /**
       * Normalise weights to unity
       */
      void normaliseWeights();

   private:
      RealVector     m_weights;
};

class MovingAverage
{
   public:
      MovingAverage( double width );
      virtual ~MovingAverage();

      TwoTuple calculate( const TwoTuple& data ) const;

   private:
      virtual double weightFunc( double x ) const;

   private:
      double         m_width;
};




} /// namespace Math

#endif // MOVINGAVERAGE_H
