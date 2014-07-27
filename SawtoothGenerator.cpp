#include "SawtoothGenerator.h"

#include <math.h>

namespace Synthesizer
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SawtoothGenerator::SawtoothGenerator( const SamplingInfo& samplingInfo ) :
   IGenerator( samplingInfo )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// generate
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RawPcmData::Ptr SawtoothGenerator::generate( size_t length )
{
   RawPcmData* result = new RawPcmData( getSamplingInfo(), length );

   double phase = getModPhase();
   double val = -1 + phase / M_PI;
   double valStep = getSamplingInfo().getPhaseStepPerSample( getFrequency() ) / M_PI;

   for ( size_t i = 0; i < length; ++i )
   {
      if ( val > 1 )
      {
         val = -2 + val;
      }
      (*result)[i] = val * getCurrentSampleAmplitude();
      val += valStep;
      nextSample();
   }

   phase = ( val + 1 ) * M_PI;
   setPhase( phase );

   return RawPcmData::Ptr( result );
}

} /// namespace WaveAnalysis
