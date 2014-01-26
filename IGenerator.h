#ifndef IGENERATOR_H
#define IGENERATOR_H

#include "ISynthEnvelope.h"
#include "Note.h"
#include "RawPcmData.h"

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
       * Constructor
       */
      IGenerator( const SamplingInfo& samplingInfo );
      /**
       * Virtual destructor
       */
      virtual ~IGenerator();

      /**
       * Generate produces generated sound data. It is expected to also update the phase at the end.
       * This function should be overridden by derived classes
       */
      virtual RawPcmData::Ptr generate( size_t length ) = 0;

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
       * Set the envelope
       * The envelope is owned by the IGenerator class
       */
      void setEnvelope( ISynthEnvelope* envelope );
      /**
       * Resets the envelope phase (this effectively starts a new note)
       */
      void resetEnvelopePhase();

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
      /**
       * Get the envelope phase
       */
      size_t getEnvelopePhase() const
      {
         return m_envelopePhase;
      }

      /**
       * Should be called after a sample has been generated
       */
      void nextSample();
      /**
       * Get current sample amplitude ( = amplitude * envelope )
       */
      double getCurrentSampleAmplitude() const;

      /**
       * Get the envelope
       */
      const ISynthEnvelope& getEnvelope() const;
      ISynthEnvelope& getEnvelope();
      /**
       * Get the SamplingInfo object
       */
      const SamplingInfo& getSamplingInfo() const;
      SamplingInfo& getSamplingInfo();

   protected:
      ISynthEnvelope*  m_envelope;
      SamplingInfo     m_samplingInfo;
      double           m_amplitude;
      double           m_frequency;
      double           m_phase;
      size_t           m_envelopePhase;

   /**
    * Blocked copy-constructor and assignment
    */
   private:
      IGenerator( const IGenerator& other );
      IGenerator& operator=( const IGenerator& other );

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

inline void IGenerator::resetEnvelopePhase()
{
   m_envelopePhase = 0;
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

inline const SamplingInfo& IGenerator::getSamplingInfo() const
{
   return m_samplingInfo;
}

inline SamplingInfo& IGenerator::getSamplingInfo()
{
   return m_samplingInfo;
}

inline void IGenerator::nextSample()
{
   ++m_envelopePhase;
}

inline double IGenerator::getCurrentSampleAmplitude() const
{
   return m_amplitude * m_envelope->getEnvelope( m_envelopePhase );
}

} /// End of namespace Synthesizer

#endif // IGENERATOR_H
