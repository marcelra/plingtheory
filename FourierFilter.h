#ifndef FOURIERFILTER_H
#define FOURIERFILTER_H

#include "SamplingInfo.h"
#include "RawPcmData.h"

#include <cstddef>

namespace WaveAnalysis {

class FourierFilter
{
   public:
      FourierFilter( double frequency, size_t nPeriods, const SamplingInfo& samplingInfo );

      RawPcmData::Ptr apply( const RawPcmData& data ) const;

   private:
      void init();

      double        m_frequency;
      double        m_nPeriods;
      SamplingInfo  m_samplingInfo;
      size_t        m_nSamplesBase;

};

} /// namespace WaveAnalysis

#endif // FOURIERFILTER_H
