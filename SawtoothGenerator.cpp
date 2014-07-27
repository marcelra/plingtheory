#include "SawtoothGenerator.h"

#include <math.h>

namespace Synthesizer
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SawtoothGenerator::SawtoothGenerator( const SamplingInfo& samplingInfo ) :
   AdditiveSynthesizer( samplingInfo )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getHarmonicsInfo
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector< AdditiveSynthesizer::HarmonicInfo > SawtoothGenerator::getHarmonicsInfo() const
{
   size_t iMaxHarmonic = getSamplingInfo().getNyquistFrequency() / getFrequency();

   std::vector< HarmonicInfo > result;
   for ( size_t iHarmonic = 1; iHarmonic < iMaxHarmonic; ++iHarmonic )
   {
      double phaseStep = getSamplingInfo().getPhaseStepPerSample( getFrequency() * iHarmonic );
      double phase = getPhase() * iHarmonic;
      double amplitude = 2 / M_PI / iHarmonic * ( ( iHarmonic % 2 == 0 ) ? -1 : 1 );
      result.push_back( HarmonicInfo( phaseStep, phase, amplitude ) );
   }

   return result;
}

} /// namespace WaveAnalysis
