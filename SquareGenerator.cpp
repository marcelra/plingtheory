#include "SquareGenerator.h"

#include <math.h>

namespace Synthesizer {

SquareGenerator::SquareGenerator( const SamplingInfo& samplingInfo ) :
   IGenerator( samplingInfo )
{}

RawPcmData::Ptr SquareGenerator::generate( size_t length )
{
   RawPcmData* data = new RawPcmData( getSamplingInfo(), length );

   double phaseStep = getSamplingInfo().getPhaseStepPerSample( getFrequency() );
   double phase = getPhase();
   double amplitude = getAmplitude();
   const ISynthEnvelope& envelope = getEnvelope();

   for ( size_t iSample = 0; iSample < length; ++iSample )
   {
      phase += phaseStep;
      double currentAmp = amplitude * envelope.getEnvelope( iSample );
      (*data)[iSample] = sin( phase ) > 0? currentAmp : -currentAmp;
   }

   setPhase( phase );
   return RawPcmData::Ptr( data );
}

} /// namespace Synthesizer
