#ifndef NOTE_H
#define NOTE_H

#include "Duration.h"

#include <cstddef>
#include <string>

namespace Music {

/**
 * @class Note
 * @brief Class describing a note with by a pitch, octave number and duration
 * @see Duration
 */
class Note
{
   public:
      /**
       * Base pitches (only piano-tuning is supported at the moment)
       */
      enum PitchClass
      {
         C = 0,
         Cis = 1,
         Des = 1,
         D = 2,
         Dis = 3,
         Es = 3,
         E = 4,
         F = 5,
         Fis = 6,
         Ges = 6,
         G = 7,
         Gis = 8,
         As = 8,
         A = 9,
         Ais = 10,
         Bes = 10,
         B = 11
      };

      /**
       * Create a note
       */
      Note( PitchClass pitch, size_t octave, const Duration& duration = Duration(1.0) );

      /**
       * Create the note closest to pitch given by frequency
       */
      Note( double frequency, const Duration& duration = Duration(1.0) );

      /**
       * Destructor
       */
      virtual ~Note();

      /**
       * Calculate the frequency
       */
      double getFrequency() const;
      /**
       * Get the duration object. @see Duration
       */
      const Duration& getDuration() const;
      /**
       * Get a string representation of the note (e.g. "A4")
       */
      std::string toString() const;

      /**
       * modifyPitch
       */
      void modifyPitch( int numSemitones );

      /**
       * Get the tuning (default: A4 = 440Hz)
       */
      static double getTuningA4();

   private:
      /**
       * Helper function for getting the base pitch C0, D0, etc.
       */
      double getBasePitch( PitchClass pitch ) const;
      /**
       * Get multiplier for @param octave (i.e. 2^octave)
       */
      double getOctaveMultiplier( size_t octave ) const;


   private:
      PitchClass      m_pitch;         //! Base pitch
      size_t          m_octave;        //! Octave
      Duration        m_duration;      //! Duration

      static const double   s_basePitchList[12];    //! Pre-calculated base pitches
      static const double   s_tuningA4;
};

} /// namespace Music

#endif // INOTE_H
