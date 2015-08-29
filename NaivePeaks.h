#ifndef FEATUREALGORITHM_NAIVEPEAKS_H
#define FEATUREALGORITHM_NAIVEPEAKS_H

#include "RealVector.h"

#include <cstddef>

/// Forward declares
namespace Feature
{
   class Peak;
}

namespace FeatureAlgorithm
{

/**
 * @class NaivePeaks
 * @brief Simple algorithm to find peaks in real-valued data
 *
 * Peak is defined by change of derivative
 * Pedestal is determined by surrounding peaks. Since the left and right pedestals can be of unequal height, interpolation
 * is done to find the pedestal below the peak centre.
 */
class NaivePeaks
{
   public:
      /**
       * @enum Mode
       * @brief Indicate what type of peaks should be detected.
       */
      enum Mode
      {
         Max = 0,
         Min,
         MaxAndMin
      };

      class Monitor
      {
         public:
            RealVector signChangeIndices;
      };

   public:
      /**
       * Constructor
       * @param realArray: is not owned and should not be deleted until the last execute
       * @param mode: @see Mode
       * @param maxNumPeaks: The maximum number of peaks, selected by prominence
       */
      NaivePeaks( Mode mode, size_t maxNumPeaks = 0 );

      /**
       * Worker method
       */
      std::vector< Feature::Peak* > execute( const RealVector& realArray, Monitor* monitor = 0 );

   private:
      /**
       * Calculate the half width of the peak (TODO: not yet implemented)
       */
      double calculateHalfWidth( const RealVector& realArray, size_t position, const std::vector< size_t >& peakPositions, size_t posVecIndex ) const;

   /**
    * Reference to the constructor parameters
    */
   private:
      Mode                        m_mode;
      size_t                      m_maxNumPeaks;

   /**
    * Helper data
    */
   private:
      std::vector< double > m_pedestalCentre;
};

} /// namespace FeatureAlgorithm

#endif // FEATUREALGORITHM_NAIVEPEAKS_H
