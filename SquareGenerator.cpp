#include "SquareGenerator.h"

#include <math.h>

namespace Synthesizer {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SquareGenerator::SquareGenerator( const SamplingInfo& samplingInfo ) :
   AdditiveSynthesizer( samplingInfo )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getHarmonicsInfo
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector< AdditiveSynthesizer::HarmonicInfo > SquareGenerator::getHarmonicsInfo() const
{
   size_t nHarmonics = getSamplingInfo().getNyquistFrequency() / getFrequency();

   std::vector< HarmonicInfo > result;

   for ( size_t iHarmonic = 1; iHarmonic < nHarmonics; iHarmonic += 2 )
   {
      double relAmp = 4 / M_PI / iHarmonic;
      double phase = fmod( getPhase() * iHarmonic, 2 * M_PI );
      double phaseStep = getSamplingInfo().getPhaseStepPerSample( getFrequency() * iHarmonic );

      result.push_back( HarmonicInfo( phaseStep, phase, relAmp ) );
   }

   return result;
}

} /// namespace Synthesizer
