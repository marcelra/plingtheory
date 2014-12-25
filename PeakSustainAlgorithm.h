#ifndef PEAKSUSTAINALGORITHM_H
#define PEAKSUSTAINALGORITHM_H

#include "AlgorithmBase.h"
#include "IBasicSpectrumPeak.h"

#include <vector>

namespace Feature
{

/**
 * @class SustainedPeak
 * @brief Class describing a couple of peaks with compatible frequencies in subsequent STFT bins glued together in time.
 * @note The peaks must be connected without gaps in chronologically ordered. So first peak first, last peak last.
 * @note The peaks are not owned by this class. This means that if the peaks are destructed, the individual peaks cannot
 * be queried anymore.
 */
class SustainedPeak : IBasicSpectrumPeak
{
   public:
      /**
       * Constructor. Requires at least one peak @param firstPeak.
       */
      SustainedPeak( const IBasicSpectrumPeak* firstPeak );

      /**
       * Add another peak, @param nextPeak, to the current sustained peak.
       */
      void connectPeak( const IBasicSpectrumPeak* nextPeak );
      /**
       * Finish the sustained peak. This method needs to be called in order to calculate derived quantities such as the
       * mean frequency of all glued peaks and the mean height of all the peaks.
       */
      void finishBuilding();

      /**
       * Query the individual peaks.
       */
      const std::vector< const IBasicSpectrumPeak* >& getAllPeaks() const;

   /**
    * IBasicSpectrumPeak interface.
    */
   public:

      /**
       * Returns the mean frequency of all the peaks that are connected.
       */
      double getFrequency() const;
      /**
       * Returns the mean height of all the peaks that are connected.
       */
      double getHeight() const;
      /**
       * Returns zero for the moment (TODO).
       */
      double getFrequencyUncertainty() const;

      /**
       * Returns the start of the first connected peak in samples.
       */
      size_t getStartTimeSamples() const;
      /**
       * Returns the end of the last connected peak in samples.
       */
      size_t getEndTimeSamples() const;

   private:
      std::vector< const IBasicSpectrumPeak* >  m_connectedPeaks;       //! All peaks that comprise the sustained peak.
      double                                    m_meanFrequency;        //! Mean peak frequency.
      double                                    m_meanHeight;           //! Mean peak height.
};

} /// namespace Feature

namespace FeatureAlgorithm
{

/**
 * @class PeakSustainAlgorithm
 * @brief Algorithm for finding peaks in subsequent STFT windows that have similar frequencies. The result represents
 * sinusoids that span multiple Fourier windows.
 */
class PeakSustainAlgorithm : public AlgorithmBase
{
   public:
      /**
       * Constructor. For arguments @see AlgorithmBase.
       */
      PeakSustainAlgorithm( const std::string& name = "PeakSustainAlgorithm", const AlgorithmBase* parent = 0 );

      /**
       * Execute the algorithm on extracted peaks from an STFT. The first vector index represent the STFT spectrum index,
       * the second index represents the index of the peak found in a particular spectrum.
       */
      std::vector< Feature::SustainedPeak* > execute( const std::vector< std::vector< Feature::IBasicSpectrumPeak* > >& peaks );

};

} /// namespace FeatureAlgorithm


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods SustainedPeak
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Feature
{

inline const std::vector< const IBasicSpectrumPeak* >& SustainedPeak::getAllPeaks() const
{
   return m_connectedPeaks;
}

} /// namespace Feature

#endif // PEAKSUSTAINALGORITHM_H
