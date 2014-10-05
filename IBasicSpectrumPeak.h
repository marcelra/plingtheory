#ifndef IBASICSPECTRUMPEAK_H
#define IBASICSPECTRUMPEAK_H

namespace Feature
{

class IBasicSpectrumPeak
{
   public:
      virtual ~IBasicSpectrumPeak() {}

   public:
      virtual double getFrequency() const = 0;
      virtual double getHeight() const = 0;
      virtual double getFrequencyUncertainty() const = 0;
};

} /// namespace Feature

#endif // IBASICSPECTRUMPEAK_H
