#include "SineGenerator.h"

#include <math.h>

namespace Synthesizer {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SineGenerator::SineGenerator( const SamplingInfo& samplingInfo ) :
   IGenerator( samplingInfo )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// generate
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RawPcmData::Ptr SineGenerator::generate( size_t length )
{
   RawPcmData* data = new RawPcmData( getSamplingInfo(), length );

   double phaseStep = getSamplingInfo().getPhaseStepPerSample( getFrequency() );
   double phase = getPhase();

   for ( size_t iSample = 0; iSample < length; ++iSample )
   {
      phase += phaseStep;
      double val = sin( phase ) * getCurrentSampleAmplitude();
      (*data)[iSample] = val;
      nextSample();
   }

   setPhase( phase );
   return RawPcmData::Ptr( data );
}

} /// namespace Synthesizer
