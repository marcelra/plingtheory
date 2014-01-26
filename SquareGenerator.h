#ifndef SQUAREGENERATOR_H
#define SQUAREGENERATOR_H

#include "IGenerator.h"

namespace Synthesizer {

/**
 * @class SquareGenerator
 * @brief Class that generates a square wave
 */
class SquareGenerator : public IGenerator
{
   public:
      /**
       * constructor (@see IGenerator)
       */
      SquareGenerator( const SamplingInfo& samplingInfo );
      /**
       * generate (@see IGenerator)
       */
      RawPcmData::Ptr generate( size_t length );
};

} /// namespace Synthesizer

#endif // SQUAREGENERATOR_H
