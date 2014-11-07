#ifndef IBASICSPECTRUMPEAK_H
#define IBASICSPECTRUMPEAK_H

#include <cstddef>

namespace Feature
{

class IBasicSpectrumPeak
{
   public:
      virtual ~IBasicSpectrumPeak() {}

   public:
      /**
       * Get the frequency of the peak.
       */
      virtual double getFrequency() const = 0;

      /**
       * Get the height of the peak.
       */
      virtual double getHeight() const = 0;

      /**
       * Get the frequency uncertainty.
       */
      virtual double getFrequencyUncertainty() const = 0;

      /**
       * Get the start time of this peak expressed in samples.
       */
      virtual size_t getStartTimeSamples() const = 0;
      /**
       * Get the end time of this peak expressed in samples.
       */
      virtual size_t getEndTimeSamples() const = 0;
};

} /// namespace Feature

#endif // IBASICSPECTRUMPEAK_H
