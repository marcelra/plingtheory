#ifndef MONOPHONICSIMPLERANDOMMUSICGENERATOR_H
#define MONOPHONICSIMPLERANDOMMUSICGENERATOR_H

#include "Note.h"
#include "RawPcmData.h"

#include "TRandom3.h"

namespace Synthesizer {
   class IGenerator;
}

namespace Music {

/**
 * @class MonophonicSimpleRandomMusicGenerator
 * @brief Generates random music
 *
 * From a set of given notes it will draw randomly a note. These notes are generated with a configurable synthesizer
 * and form a RawPcmData when generateRandomMusic is called.
 */
class MonophonicSimpleRandomMusicGenerator
{
   public:
      /**
       * Constructor
       * @param availableNotes: indicate which notes are used to draw randomly from
       * @param seed: random number seed
       */
      MonophonicSimpleRandomMusicGenerator( const std::vector<Note>& availableNotes, int seed );

      /**
       * Assign a synthesizer @param generator to use for the generation @see Synthesizer
       * The synthesizer will not be owned by the algorithm
       */
      void useSynthesizer( Synthesizer::IGenerator* generator );

      /**
       * Generate a piece of random music. @param numNotes notes will be generated.
       * If the @param generatedNotesResult pointer is given, the actual notes that were generated are stored in this vector
       */
      RawPcmData::Ptr generateRandomMusic( size_t numNotes, std::vector< Note >* generatedNotesResult = 0 );

   private:
      std::vector<Note>        m_availableNotes;      //! Urn of notes to draw from
      TRandom3                 m_random;              //! Random number generator
      Synthesizer::IGenerator* m_synth;               //! Synthesizer

};

} /// namespace Music

#endif // MONOPHONICSIMPLERANDOMMUSICGENERATOR_H
