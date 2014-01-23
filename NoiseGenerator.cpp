#include "NoiseGenerator.h"

namespace Synthesizer
{

NoiseGenerator::NoiseGenerator( const SamplingInfo& samplingInfo, int seed ) :
   IGenerator( samplingInfo ),
   m_ranGen( seed >= 0 ? seed : s_ranSeed++ )
{}

RawPcmData::Ptr NoiseGenerator::generate( size_t length )
{
   RawPcmData* result = new RawPcmData( getSamplingInfo(), length );

   double amplitude = getAmplitude();
   const ISynthEnvelope& envelope = getEnvelope();

   for ( size_t iSample = 0; iSample < length; ++iSample )
   {
      double val = m_ranGen.Uniform( -1, 1 ) * amplitude * envelope.getEnvelope( iSample );
      (*result)[iSample] = val;
   }

   return RawPcmData::Ptr( result );
}

int NoiseGenerator::s_ranSeed = 0;

} /// namespace Synthesizer
