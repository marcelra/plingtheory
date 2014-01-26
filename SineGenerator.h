#ifndef SINEGENERATOR_H
#define SINEGENERATOR_H

#include "IGenerator.h"

namespace Synthesizer {

/**
 * @class SineGenerator
 * @brief Class that generates a sine wave
 */
class SineGenerator : public IGenerator
{
   public:
      /**
       * constructor (@see IGenerator)
       */
      SineGenerator( const SamplingInfo& samplingInfo );
      /**
       * generate (@see IGenerator)
       */
      RawPcmData::Ptr generate( size_t length );
};

} /// namespace Synthesizer

#endif // SINEGENERATOR_H
