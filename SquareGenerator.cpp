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

   size_t highestHarmonic = getSamplingInfo().getNyquistFrequency() / getFrequency();

   for ( size_t iSample = 0; iSample < length; ++iSample )
   {
      double osc = 0;
      for ( size_t iHarmonic = 1; iHarmonic < highestHarmonic; iHarmonic += 2 )
      {
         /// TODO: phase information ignored.
         osc += 1. / iHarmonic * sin( phaseStep * iSample * iHarmonic );
      }
      double currentAmp = getCurrentSampleAmplitude();
      osc *= 4 / M_PI * currentAmp;
      (*data)[iSample] = osc;

      nextSample();
   }

   phase += phaseStep * length;
   setPhase( phase );
   return RawPcmData::Ptr( data );
}

} /// namespace Synthesizer
