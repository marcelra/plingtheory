#ifndef SAWTOOTHGENERATOR_H
#define SAWTOOTHGENERATOR_H

#include "AdditiveSynthesizer.h"

namespace Synthesizer
{

/// TODO: doc
class SawtoothGenerator : public AdditiveSynthesizer
{
   public:
      /**
       *  Constructor (@see IGenerator).
       */
      SawtoothGenerator( const SamplingInfo& samplingInfo );

   private:
      /**
       * getHarmonicsInfo (@see AdditiveSynthesizer).
       */
      std::vector< HarmonicInfo > getHarmonicsInfo() const;
};

} /// namespace Synthesizer

#endif // SAWTOOTHGENERATOR_H
