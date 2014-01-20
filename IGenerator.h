#ifndef IGENERATOR_H
#define IGENERATOR_H

#include "RawPcmData.h"
#include "Note.h"

#include <vector>

namespace Synthesizer {

/**
 * @class IGenerator
 * @brief Abstract class that lays down an interface for generators (synthesizers)
 */
class IGenerator
{
   public:
      /**
       * Virtual destructor
       */
      virtual ~IGenerator();

      /**
       * Generate produces generated sound data. It is expected to also update the phase at the end.
       * This function should be overridden by derived classes
       */
      virtual RawPcmData::Ptr generate( size_t length, const SamplingInfo& samplingInfo ) = 0;

      /**
       * Generate a sequence of notes (TODO: test)
       */
      RawPcmData::Ptr generate( const std::vector< Music::Note >& notes, double bpm, const SamplingInfo& samplingInfo );

      /**
       * Set the amplitude of the generator
       */
      void setAmplitude( double amplitude );
      /**
       * Set the frequency
       */
      void setFrequency( double frequency );
      /**
       * Set the phase
       */
      void setPhase( double phase );

      /**
       * Obtain the amplitude (i.e. max-value, not the current amplitude)
       */
      double getAmplitude() const;
      /**
       * Obtain the frequency
       */
      double getFrequency() const;
      /**
       * Obtain the phase. This phase is recommended to reflect the current phase.
       */
      double getPhase() const;

   protected:
      double         m_amplitude;
      double         m_frequency;
      double         m_phase;
};

////////////////////////////////////////////////////////////////////////////////
/// Inline functions IGenerator
////////////////////////////////////////////////////////////////////////////////
inline void IGenerator::setAmplitude( double amplitude )
{
   m_amplitude = amplitude;
}

inline void IGenerator::setFrequency( double frequency )
{
   m_frequency = frequency;
}

inline void IGenerator::setPhase( double phase )
{
   m_phase = phase;
}

inline double IGenerator::getAmplitude() const
{
   return m_amplitude;
}

inline double IGenerator::getFrequency() const
{
  return m_frequency;
}

inline double IGenerator::getPhase() const
{
   return m_phase;
}

} /// End of namespace Synthesizer

#endif // IGENERATOR_H
