#ifndef ANALYSISSRPA_H
#define ANALYSISSRPA_H

#include "AlgorithmBase.h"
#include "SamplingInfo.h"

#include <vector>

namespace Analysis
{

/**
 * @class AnalysisSrpa
 * @brief Analysis of SrSpecPeakAlgorithm performance.
 * The following sub analyses are provided:
 * - studyFrequencyPerformance:
 *    Study the error of the amplitude estimate and of the frequency estimate in a sinusoidal signal of a signle frequency.
 * - studyTwinPeakPerformance:
 *    Study the error of the estimated frequency in a signal consisting of two sinusoids of a different frequency. The frequency estimate is matched
 *    to the input frequencies in order to determine the effeciency as function of the delta frequency.
 * - studyAmpBehaviour:
 *    Study the behaviour of the peak detection algorithm and the tHat term in spectral reassignment in a sinusoid singal which is linearly ramped up
 *    or ramped down.
 */
class AnalysisSrpa : public AlgorithmBase
{
   public:
      /**
       * Constructor (@see AlgorithmBase).
       */
      AnalysisSrpa( const std::string& name = "AnalysisSrpa", const AlgorithmBase* parent = 0 );

      /**
       * Set the Fourier window size and the number of samples used for zero padding.
       */
      void setFourierConfig( size_t fourierSize, size_t numSamplesZeroPadding = 0 );

   public:
      /**
       * Described in the main documentation block.
       */
      void studyFrequencyPerformance( const std::vector< double >& frequencies );
      /**
       * Described in the main documentation block.
       */
      void studyTwinPeakPerformance( const std::vector< double >& frequencies, const std::vector< double >& frequencyDifference, double amp2 );
      /**
       * Described in the main documentation block.
       */
      void studyAmpBehaviour( double frequency, double ampFracBegin, double ampFracEnd );

   private:
      size_t         m_fourierSize;       //! Fourier batch size.
      size_t         m_zeroPadSize;       //! Number of zero padding samples.
      double         m_amplitude;         //! Amplitude of the sinusoid.
      SamplingInfo   m_samplingInfo;      //! SamplingInfo object.
};

} /// namespace Analysis

#endif // ANALYSISSRPA_H
