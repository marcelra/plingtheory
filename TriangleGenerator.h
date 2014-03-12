#ifndef TRIANGLEGENERATOR_H
#define TRIANGLEGENERATOR_H

#include "IGenerator.h"

namespace Synthesizer
{

/**
 * @class TriangleGenerator
 * @brief Class that generates triangle waves
 */
class TriangleGenerator : public IGenerator
{
   public:
      /**
       * constructor (@see IGenerator)
       */
      TriangleGenerator( const SamplingInfo& samplingInfo );
      /**
       * generate (@see IGenerator)
       */
      RawPcmData::Ptr generate( size_t length );
};

} /// namespace Synthesizer

#endif // TRIANGLEGENERATOR_H
