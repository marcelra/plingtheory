#ifndef ADDITIVESYNTHESIZER_H
#define ADDITIVESYNTHESIZER_H

#include "IGenerator.h"

namespace Synthesizer
{

/**
 * @class AdditiveSynthesizer
 * @brief Base class for additive synths such as sawtooth, triangle and square oscillators.
 */
class AdditiveSynthesizer : public IGenerator
{
   public:
      /**
       * Constructor (@see IGenerator).
       */
      AdditiveSynthesizer( const SamplingInfo& samplingInfo );
      /**
       * Generate the data. Do not override this method.
       */
      RawPcmData::Ptr generate( size_t length ) final;

   protected:
      /**
       * @class HarmonicInfo
       * @brief Class that holds all necessary information to generate an harmonic.
       */
      class HarmonicInfo
      {
         public:
            HarmonicInfo( double phaseStep, double phase, double amplitude );

            /**
             * Get the phase step for this harmonic.
             */
            double getPhaseStep() const;
            /**
             * Get the initial phase for this harmonic.
             */
            double getPhase() const;
            /**
             * Get the amplitude for this harmonic.
             */
            double getAmplitude() const;

         private:
            double m_phaseStep;
            double m_phase;
            double m_amplitude;
      };

   private:
      /**
       * Get the relative amplitude of harmonic @param iHarmonic. The groundtone has iHarmonic = 0.
       */
      virtual std::vector< HarmonicInfo > getHarmonicsInfo() const = 0;

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods AdditiveSynthesizer::HarmonicInfo
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getPhaseStep
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double AdditiveSynthesizer::HarmonicInfo::getPhaseStep() const
{
   return m_phaseStep;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getPhase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double AdditiveSynthesizer::HarmonicInfo::getPhase() const
{
   return m_phase;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getAmplitude
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double AdditiveSynthesizer::HarmonicInfo::getAmplitude() const
{
   return m_amplitude;
}

} /// namespace Synthesizer

#endif // ADDITIVESYNTHESIZER_H
