#ifndef DURATION_H
#define DURATION_H

#include <cstddef>

class SamplingInfo;

namespace Music {

/**
 * @class Duration
 * @brief Simple class that scales a duration in beats to seconds
 * A quarter note has duration 1 (and not 1/4)
 */
class Duration
{
   public:
      /**
       * Create a duration object
       */
      Duration( double noteDuration );

      /**
       * Convert a duration to seconds
       */
      double getSeconds( double bpm ) const;

      /**
       * Get the number of samples
       */
      size_t getNumSamples( double bpm, const SamplingInfo& samplingInfo ) const;

   private:
      double   m_noteDuration;        //! The duration of the note in 1/4 notes
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double Duration::getSeconds( double bpm ) const
{
   return 60/bpm*m_noteDuration;
}

} /// namespace Music

#endif // DURATION_H
