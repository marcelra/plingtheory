#include "SineEnvelopeGenerator.h"

#include "RawPcmData.h"

#include <math.h>
#include <iostream>

namespace Temp {

SineEnvelopeGenerator::SineEnvelopeGenerator( double frequency, const RawPcmData& envelope ) :
   m_frequency( frequency ),
   m_envelope( envelope )
{
}

void SineEnvelopeGenerator::mixIntoBuffer( RawPcmData& buffer )
{
   double sinPhase = 0.000001;
   double prevSinPhase = -1;
   double phase = 0;
   double amp = 0;
   double phaseStep = buffer.getSamplingInfo().getPhaseStepPerSample( m_frequency );
   for ( size_t iSample = 0; iSample < m_envelope.size()*2; ++iSample )
   {
      if ( sinPhase*prevSinPhase < 0 )
      {
         amp = m_envelope[iSample/2]*m_envelope[iSample/2];
         prevSinPhase = phase;
         // std::cout << "New phase, amp = " << amp << std::endl;
      }
      prevSinPhase = sinPhase;
      buffer[iSample] += amp*sinPhase;
      // std::cout << buffer[iSample] << std::endl;
      phase += phaseStep;
      sinPhase = sin(phase);
   }
   return;
}

} /// namespace Temp
