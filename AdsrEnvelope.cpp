#include "AdsrEnvelope.h"

#include <cmath>

namespace Synthesizer {

AdsrEnvelope::AdsrEnvelope( double attack, double decay, double sustain, double sustainLevel, double release ) :
   m_attack( attack ),
   m_decay( decay ),
   m_sustain( sustain ),
   m_sustainLevel( sustainLevel ),
   m_release( release )
{
   updateDerivedQuantities();
}

double AdsrEnvelope::getAttack() const
{
   return m_attack;
}

double AdsrEnvelope::getDecay() const
{
   return m_decay;
}

double AdsrEnvelope::getSustain() const
{
   return m_sustain;
}

double AdsrEnvelope::getSustainLevel() const
{
   return m_sustainLevel;
}

double AdsrEnvelope::getRelease() const
{
   return m_release;
}

void AdsrEnvelope::setAttack( double attack )
{
   m_attack = attack;
   updateDerivedQuantities();
}

void AdsrEnvelope::setDecay( double decay )
{
   m_decay = decay;
   updateDerivedQuantities();
}

void AdsrEnvelope::setSustain( double sustain )
{
   m_sustain = sustain;
   updateDerivedQuantities();
}

void AdsrEnvelope::setSustainLevel( double sustainLevel )
{
   m_sustain = sustainLevel;
}

void AdsrEnvelope::setRelease( double release )
{
   m_release = release;
}

void AdsrEnvelope::updateDerivedQuantities()
{
   m_decayEnd = m_attack + m_decay;
   m_sustainEnd = m_decayEnd + m_sustain;
}

double AdsrEnvelope::getEnvelope( size_t iSample ) const
{
   double sampleD = iSample;
   if ( sampleD < m_attack && m_attack > 1 )
   {
      return sampleD / m_attack;
   }
   else if ( sampleD < m_decayEnd && m_decay > 1 )
   {
      return 1 + ( m_sustainLevel - 1 ) * ( 1 / m_decay ) * ( sampleD - m_attack );
   }
   else if ( m_sustain < 0 )
   {
      return m_sustainLevel;
   }
   else if ( sampleD < m_sustainEnd )
   {
      return m_sustainLevel;
   }
   else
   {
      return m_sustainLevel * exp( -( sampleD - m_sustainEnd ) / m_release );
   }
}

} /// namespace Synthesizer
