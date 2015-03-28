#ifndef TIMESTRETCHER_H
#define TIMESTRETCHER_H

#include "AlgorithmBase.h"
#include "RawPcmData.h"

/// Forward declarations.
namespace Feature {
   class SustainedPeak;
}

namespace WaveAnalysis {
   class FourierConfig;
}

namespace Music
{

class TimeStretcher : public AlgorithmBase
{
   public:
      TimeStretcher( double stretchFactor, const std::string& name = "TimeStretcher", const AlgorithmBase* parent = 0 );

      RawPcmData execute( const RawPcmData& input );

      RawPcmData generateFromSustainedPeaks( const std::vector< Feature::SustainedPeak* >& sustainedPeaks,
                                             const WaveAnalysis::FourierConfig& fourierConfig,
                                             const SamplingInfo& samplingInfo,
                                             size_t originalLength ) const;

   private:
      double      m_stretchFactor;

};

} /// namespace Synthesizer

#endif // TIMESTRETCHER_H
