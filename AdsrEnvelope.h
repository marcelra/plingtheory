#ifndef ADSRENVELOPE_H
#define ADSRENVELOPE_H

#include "ISynthEnvelope.h"

#include <stddef.h>

namespace Synthesizer {

/**
 * @class AdsrEnvelope
 * @brief Class describing an Attack, Decay, Sustain, Release envelope.
 * @see constructor for details
 */
class AdsrEnvelope : public ISynthEnvelope
{
   public:
      /**
       * constructor
       * @param attack: the number of samples in which the sound linearly ramps up to maximum amplitude 1
       * @param decay: the number of samples in which the sound linearly decays to @param sustainLevel
       * @param sustain: the number of samples in which the sound sustains at a constant level
       * @param sustainLevel: the level during sustain
       * @param release: half-life of the tone after sustain is completed in number of samples
       */
      AdsrEnvelope( double attack, double decay, double sustain, double sustainLevel, double release );

      /**
       * Self explanatory (@see constructor)
       */
      double getAttack() const;
      double getDecay() const;
      double getSustain() const;
      double getSustainLevel() const;
      double getRelease() const;

      /**
       * Self explanatory (@see constructor)
       */
      void setAttack( double attack );
      void setDecay( double decay );
      void setSustain( double sustainDuration );
      void setSustainLevel( double sustainLevel );
      void setRelease( double release );

      /**
       * Calculates the envelope of the sound.
       * @param sample: the sample index. The sample index starts at zero for a newly struck tone.
       */
      double getEnvelope( size_t sample ) const;

   private:
      /**
       * Method that calculates derived quantities during set operations in order to reduce the calculations
       * in getEnvelope()
       */
      void updateDerivedQuantities();

   private:
      double         m_attack;         //! the number of samples of the linear interpolation of the attack
      double         m_decay;          //! the number of samples of the linear interpolation of the decay
      double         m_sustain;        //! the number of samples of the sustain
      double         m_sustainLevel;   //! the level of sustain
      double         m_release;        //! the half-life of the decay

      double         m_decayEnd;       //! the number of samples from start of note until sustain sets in
      double         m_sustainEnd;     //! the number of samples from start of note until release sets in
};

} /// namespce Synthesizer

#endif // ADSRENVELOPE_H
