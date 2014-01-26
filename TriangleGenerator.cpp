#include "TriangleGenerator.h"

#include <math.h>

namespace Synthesizer
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TriangleGenerator::TriangleGenerator( const SamplingInfo& samplingInfo ) :
   IGenerator( samplingInfo )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// generate
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RawPcmData::Ptr TriangleGenerator::generate( size_t length )
{
   RawPcmData* data = new RawPcmData( getSamplingInfo(), length );
   double valStep = 2*M_PI / getSamplingInfo().getPeriodInSamples( getFrequency() );

   /// Calculate phase between (0, 2*pi)
   double phase = fmod( getPhase(), 2*M_PI );
   if ( phase < 0 )
   {
      phase += 2*M_PI;
   }

   /// Calculate val and valStep from the phase
   double val;
   if ( phase < 0.5*M_PI )
   {
      val = 0 + phase / ( 0.5*M_PI );
   }
   else if ( phase < 1.5*M_PI )
   {
      valStep = -valStep;
      val = 1 - ( phase - 0.5*M_PI ) / ( 0.5*M_PI );
   }
   else
   {
      val = -1 + ( phase - 1.5*M_PI ) / ( 0.5*M_PI );
   }

   /// Generate values
   for ( size_t iSample = 0; iSample < length; ++iSample )
   {
      val += valStep;
      if ( val > 1 )
      {
         val = 1 - ( val - 1 );
         valStep = -valStep;
      }
      else if ( val < -1 )
      {
         val = -1 - ( val + 1 );
         valStep = -valStep;
      }
      (*data)[iSample] = val * getCurrentSampleAmplitude();
      nextSample();
   }

   /// Recalculate phase from valStep and value
   if ( valStep > 0 && val > 0 )
   {
      phase = val * 0.5*M_PI;
   }
   else if ( valStep < 0 )
   {
      phase = -val * 0.5*M_PI + M_PI;
   }
   else
   {
      phase = val * 0.5*M_PI + 2*M_PI;
   }

   setPhase( phase );
   return RawPcmData::Ptr( data );
}

} /// namespace Synthesizer
