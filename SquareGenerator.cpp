#include "SquareGenerator.h"

#include <math.h>

namespace Synthesizer {

RawPcmData::Ptr SquareGenerator::generate( size_t length, const SamplingInfo& samplingInfo )
{
   RawPcmData::Ptr data( new RawPcmData( samplingInfo, length ) );

   double sampleRate = samplingInfo.getSamplingRate();

   double phaseStep = 2*M_PI*getFrequency()/sampleRate;
   double phase = getPhase();
   double amplitude = getAmplitude();

   for ( size_t iSample = 0; iSample < length; ++iSample )
   {
      phase += phaseStep;
      (*data)[iSample] = sin( phase ) > 0? amplitude : -amplitude;
   }

   setPhase( phase );
   return data;
}

} /// namespace Synthesizer
