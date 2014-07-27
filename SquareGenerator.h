#ifndef SQUAREGENERATOR_H
#define SQUAREGENERATOR_H

#include "AdditiveSynthesizer.h"

namespace Synthesizer {

/**
 * @class SquareGenerator
 * @brief Class that generates a square wave
 */
class SquareGenerator : public AdditiveSynthesizer
{
   public:
      /**
       * constructor (@see IGenerator)
       */
      SquareGenerator( const SamplingInfo& samplingInfo );

   private:
      /**
       * getHarmonicsInfo (@see AdditiveSynthesizer).
       */
      std::vector< HarmonicInfo > getHarmonicsInfo() const;
};

} /// namespace Synthesizer

#endif // SQUAREGENERATOR_H
