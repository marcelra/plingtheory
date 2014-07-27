#include "TriangleGenerator.h"

#include <math.h>

namespace
{

double sqr( double x )
{
   return x * x;
}

}

namespace Synthesizer
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TriangleGenerator::TriangleGenerator( const SamplingInfo& samplingInfo ) :
   AdditiveSynthesizer( samplingInfo )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getHarmonicsInfo
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector< AdditiveSynthesizer::HarmonicInfo > TriangleGenerator::getHarmonicsInfo() const
{
   size_t iMaxHarmonic = getSamplingInfo().getNyquistFrequency() / getFrequency();

   std::vector< HarmonicInfo > result;
   double mult = 1;
   for ( size_t iHarmonic = 1; iHarmonic < iMaxHarmonic; iHarmonic += 2 )
   {
      double phaseStep = getSamplingInfo().getPhaseStepPerSample( getFrequency() * iHarmonic );
      double phase = getPhase() * iHarmonic;
      double amplitude = 8 / sqr( M_PI ) / sqr( iHarmonic ) * mult;
      mult *= -1;

      result.push_back( HarmonicInfo( phaseStep, phase, amplitude ) );
   }

   return result;
}


} /// namespace Synthesizer
