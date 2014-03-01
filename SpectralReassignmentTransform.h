#ifndef SPECTRALREASSIGNMENTTRANSFORM_H
#define SPECTRALREASSIGNMENTTRANSFORM_H

#include "SRSpectrum.h"
#include "StftAlgorithm.h"

class SamplingInfo;
class RawPcmData;

namespace WaveAnalysis
{

/**
 * @class SpectralReassignmentTransform
 * @brief Calculates the Fourier trasnforms and performs the spectral reassigment method. Only the Hanning window is supported.
 */
class SpectralReassignmentTransform
{
   public:
      /**
       * Constructor. Similar to StftAlgorithm, @see StftAlgorithm for more details.
       */
      SpectralReassignmentTransform( const SamplingInfo& samplingInfo, size_t fourierSize, size_t numSamplesZeroPadding, double hopRate );

      /**
       * Execute the transform on data. For the Spectral reassigned transforms it is possible to query the SrSpectrum results
       * from the StftData.
       */
      StftData::Ptr execute( const RawPcmData& data );

   private:
      StftAlgorithm     m_stft;                 //! Ordinary Fourier transform
      StftAlgorithm     m_stftDerivative;       //! Window derivate-transform
      StftAlgorithm     m_stftTimeRamped;       //! Time-ramped transform
};

} /// namespace WaveAnalysis

#endif // SPECTRALREASSIGNMENTTRANSFORM_H
