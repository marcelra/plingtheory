#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

#include "IGenerator.h"

/// ROOT includes
#include "TRandom3.h"

namespace Synthesizer {

/**
 * @class NoiseGenerator
 * @brief Synthesizer that generates white noise
 */
class NoiseGenerator : public IGenerator
{
   public:
      /**
       * constructor (@see IGenerator)
       * @param seed: supply seed for random number generator. Seed will be unique for every instance of NoiseGenerator
       * if the value given is smaller than 0
       */
      NoiseGenerator( const SamplingInfo& samplingInfo, int seed = -1 );
      /**
       * generate (@see IGenerator)
       */
      RawPcmData::Ptr generate( size_t length );

   private:
      TRandom3    m_ranGen;
      static int  s_ranSeed;
};

} /// namespace Synthesizer

#endif // NOISEGENERATOR_H
