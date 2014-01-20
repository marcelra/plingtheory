#include "SineGenerator.h"

#include <math.h>

namespace Synthesizer {

RawPcmData::Ptr SineGenerator::generate( size_t length, const SamplingInfo& samplingInfo )
{
   RawPcmData* data = new RawPcmData( samplingInfo, length );

   double sampleRate = samplingInfo.getSamplingRate();

   double phaseStep = 2*M_PI*getFrequency()/sampleRate;
   double phase = getPhase();
   double amplitude = getAmplitude();

   for ( size_t iSample = 0; iSample < length; ++iSample )
   {
      double val = amplitude * sin( phase );
      phase += phaseStep;
      (*data)[iSample] = val;
   }

   setPhase( phase );
   return RawPcmData::Ptr( data );
}

} /// namespace Synthesizer
