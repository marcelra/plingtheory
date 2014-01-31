#ifndef SPECTRALREASSIGNMENTTRANSFORM_H
#define SPECTRALREASSIGNMENTTRANSFORM_H

#include "SRSpectrum.h"
#include "StftAlgorithm.h"

class SamplingInfo;
class RawPcmData;

namespace WaveAnalysis
{

class SpectralReassignmentTransform
{
   public:
      SpectralReassignmentTransform( const SamplingInfo& samplingInfo, size_t fourierSize, size_t numSamplesZeroPadding, double hopRate );

      RawStftData::Ptr execute( const RawPcmData& data );

   private:
      StftAlgorithm     m_stft;
      StftAlgorithm     m_stftDerivative;
      StftAlgorithm     m_stftTimeRamped;
};

} /// namespace WaveAnalysis

#endif // SPECTRALREASSIGNMENTTRANSFORM_H
