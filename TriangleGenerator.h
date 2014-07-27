#ifndef TRIANGLEGENERATOR_H
#define TRIANGLEGENERATOR_H

#include "AdditiveSynthesizer.h"

namespace Synthesizer
{

/**
 * @class TriangleGenerator
 * @brief Class that generates triangle waves
 */
class TriangleGenerator : public AdditiveSynthesizer
{
   public:
      /**
       * constructor (@see IGenerator)
       */
      TriangleGenerator( const SamplingInfo& samplingInfo );

   private:
      /**
       * getHarmonicsInfo (@see AdditiveSynthesizer).
       */
      std::vector< HarmonicInfo > getHarmonicsInfo() const;
};

} /// namespace Synthesizer

#endif // TRIANGLEGENERATOR_H
