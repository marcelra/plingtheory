#include "SineGenerator.h"

#include <math.h>

namespace Synthesizer {

SineGenerator::SineGenerator( const SamplingInfo& samplingInfo ) :
   IGenerator( samplingInfo )
{}

RawPcmData::Ptr SineGenerator::generate( size_t length )
{
   RawPcmData* data = new RawPcmData( getSamplingInfo(), length );

   double phaseStep = getSamplingInfo().getPhaseStepPerSample( getFrequency() );
   double phase = getPhase();
   double amplitude = getAmplitude();
   const ISynthEnvelope& envelope = getEnvelope();

   for ( size_t iSample = 0; iSample < length; ++iSample )
   {
      phase += phaseStep;
      double val = amplitude * envelope.getEnvelope( iSample ) * sin( phase );
      (*data)[iSample] = val;
   }

   setPhase( phase );
   return RawPcmData::Ptr( data );
}

} /// namespace Synthesizer
