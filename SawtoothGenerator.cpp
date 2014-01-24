#include "SawtoothGenerator.h"

#include <math.h>

/// TODO: remove
#include <iostream>

namespace Synthesizer
{

SawtoothGenerator::SawtoothGenerator( const SamplingInfo& samplingInfo ) :
   IGenerator( samplingInfo )
{}

RawPcmData::Ptr SawtoothGenerator::generate( size_t length )
{
   RawPcmData* data = new RawPcmData( getSamplingInfo(), length );

   double phase = getPhase();
   double amplitude = getAmplitude();
   const ISynthEnvelope& envelope = getEnvelope();

   double val = 0;
   double valStep = 2 * M_PI / getSamplingInfo().getPeriodInSamples( getFrequency() );
   std::cout << "valStep = " << valStep << std::endl;

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
      (*data)[iSample] = /*envelope.getEnvelope( iSample ) **/ amplitude * val;
   }

   if ( valStep > 0 && val > 0 )
   {
      phase = val * M_PI * 0.5;
   }
   else if ( valStep < 0 )
   {
      phase = -val * M_PI * 0.5 + M_PI;
   }
   else
   {
      phase = 2 * M_PI + val;
   }

   setPhase( phase );
   return RawPcmData::Ptr( data );
}

} /// namespace Synthesizer
