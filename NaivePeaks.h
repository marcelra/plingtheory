#ifndef FEATUREALGORITHM_NAIVEPEAKS_H
#define FEATUREALGORITHM_NAIVEPEAKS_H

#include <vector>
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

   public:
      /**
       * Constructor
       * @param realArray: is not owned and should not be deleted until the last execute
       * @param mode: @see Mode
       * @param maxNumPeaks: The maximum number of peaks, selected by prominence
       */
      NaivePeaks( const std::vector<double>& realArray, Mode mode, size_t maxNumPeaks = 0 );
      /**
       * Destructor
       */
      ~NaivePeaks();

      /**
       * Get the number of peaks detected
       */
      size_t getNumPeaks() const;
      /**
       * Get the index-th peak
       */
      const Feature::Peak& getPeak( size_t index ) const;
      /**
       * Get all peaks
       */
      const std::vector< Feature::Peak* >& getAllPeaks() const;
      /**
       * Worker method
       */
      void execute();

   private:
      /**
       * Calculate the half width of the peak (TODO: not yet implemented)
       */
      double calculateHalfWidth( size_t position, const std::vector< size_t >& peakPositions, size_t posVecIndex ) const;

   /**
    * Reference to the constructor parameters
    */
   private:
      const std::vector<double>&  m_realArray;
      Mode                        m_mode;
      size_t                      m_maxNumPeaks;

   /**
    * Results
    */
   private:
      std::vector< Feature::Peak* > m_peaks;

   /**
    * Helper data
    */
   private:
      std::vector< double > m_pedestalCentre;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods (getNumPeaks)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline size_t NaivePeaks::getNumPeaks() const
{
   return m_peaks.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods (getPeak)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Feature::Peak& NaivePeaks::getPeak( size_t index ) const
{
   return *m_peaks[ index ];
}

inline const std::vector< Feature::Peak* >& NaivePeaks::getAllPeaks() const
{
   return m_peaks;
}


} /// namespace FeatureAlgorithm

#endif // FEATUREALGORITHM_NAIVEPEAKS_H
