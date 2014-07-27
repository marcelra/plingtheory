#include "AdditiveSynthesizer.h"

#include <cmath>

#include <iostream>

namespace Synthesizer
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AdditiveSynthesizer::HarmonicInfo constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AdditiveSynthesizer::HarmonicInfo::HarmonicInfo( double phaseStep, double phase, double amplitude ) :
   m_phaseStep( phaseStep ),
   m_phase( phase ),
   m_amplitude( amplitude )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AdditiveSynthesizer methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AdditiveSynthesizer::AdditiveSynthesizer( const SamplingInfo& samplingInfo ) :
   IGenerator( samplingInfo )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// generate
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RawPcmData::Ptr AdditiveSynthesizer::generate( size_t length )
{
   RawPcmData* result = new RawPcmData( getSamplingInfo(), length );

   const std::vector< HarmonicInfo >& harmonicsInfo = getHarmonicsInfo();
   assert( harmonicsInfo.size() > 0 );

   std::vector< double > phase( harmonicsInfo.size() );
   for ( size_t iHarmonic = 0; iHarmonic < harmonicsInfo.size(); ++iHarmonic )
   {
      phase[ iHarmonic ] = harmonicsInfo[ iHarmonic ].getPhase();
   }

   for ( size_t iSample = 0; iSample < length; ++iSample )
   {
      double val = 0;
      for ( size_t iHarmonic = 0; iHarmonic < harmonicsInfo.size(); ++iHarmonic )
      {
         phase[ iHarmonic ] += harmonicsInfo[ iHarmonic ].getPhaseStep();
         val += harmonicsInfo[ iHarmonic ].getAmplitude() * sin( phase[ iHarmonic ] );
      }
      (*result)[ iSample ] = val * getCurrentSampleAmplitude();
      nextSample();
   }

   setPhase( fmod( phase[ 0 ], 2 * M_PI ) );

   return RawPcmData::Ptr( result );
}

} /// namespace Synthesizer
