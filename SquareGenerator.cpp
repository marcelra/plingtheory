#include "SquareGenerator.h"

#include <math.h>

namespace Synthesizer {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SquareGenerator::SquareGenerator( const SamplingInfo& samplingInfo ) :
   IGenerator( samplingInfo )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// generate
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RawPcmData::Ptr SquareGenerator::generate( size_t length )
{
   RawPcmData* data = new RawPcmData( getSamplingInfo(), length );

   double phaseStep = getSamplingInfo().getPhaseStepPerSample( getFrequency() );
   double phase = getPhase();

   for ( size_t iSample = 0; iSample < length; ++iSample )
   {
      phase += phaseStep;
      double currentAmp = getCurrentSampleAmplitude();
      (*data)[iSample] = sin( phase ) > 0? currentAmp : -currentAmp;
      nextSample();
   }

   setPhase( phase );
   return RawPcmData::Ptr( data );
}

} /// namespace Synthesizer
