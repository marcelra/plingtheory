#ifndef WINDOWLOCATION_H
#define WINDOWLOCATION_H

#include <cstddef>

namespace WaveAnalysis
{

/**
 * @class WindowLocation
 * @brief Helper class that remembers the window location of a STFT spectrum.
 * @note Copy constructor is used!
 */
class WindowLocation
{
   public:
      /**
       * Constructor. @param firstSampleIndex is the first sample on which the STFT window is applied.
       * @param lastSampleIndex is the last sample on which the STFT window is applied.
       */
      WindowLocation( size_t firstSampleIndex, size_t lastSampleIndex );
      /**
       * Get the first sample on which the window function was applied.
       */
      size_t getFirstSample() const;
      /**
       * Get the last sample on which the window function was applied.
       */
      size_t getLastSample() const;
      /**
       * Get the center of the window.
       */
      size_t getCentre() const;

   private:
      size_t         m_first;       //! First sample on which the window is applied.
      size_t         m_last;        //! Last sample on which the window is applied.
};


} /// namespace WaveAnalysis

#endif // WINDOWLOCATION_H
