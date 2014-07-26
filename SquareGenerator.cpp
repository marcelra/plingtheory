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

   std::vector< double > phaseVec( highestHarmonic );
   for ( size_t iHarmonic = 1; iHarmonic < highestHarmonic; iHarmonic += 2 )
   {
      phaseVec[ iHarmonic ] = fmod( phase * iHarmonic, 2 * M_PI );
   }

   for ( size_t iSample = 0; iSample < length; ++iSample )
   {
      double osc = 0;
      for ( size_t iHarmonic = 1; iHarmonic < highestHarmonic; iHarmonic += 2 )
      {
         osc += 1. / iHarmonic * sin( phaseVec[ iHarmonic ] + phaseStep * iSample * iHarmonic );
      }
      double currentAmp = getCurrentSampleAmplitude();
      osc *= 4 / M_PI * currentAmp;
      (*data)[iSample] = osc;

      nextSample();
   }

   phase += phaseStep * length;
   setPhase( fmod( phase, 2 * M_PI ) );
   return RawPcmData::Ptr( data );
}

} /// namespace Synthesizer
