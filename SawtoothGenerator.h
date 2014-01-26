#ifndef SAWTOOTHGENERATOR_H
#define SAWTOOTHGENERATOR_H

#include "IGenerator.h"

namespace Synthesizer
{

/**
 * @class SawtoothGenerator
 * @brief Class that generates triangle waves
 * TODO: rename class to TriangleGenerator
 */
class SawtoothGenerator : public IGenerator
{
   public:
      /**
       * constructor (@see IGenerator)
       */
      SawtoothGenerator( const SamplingInfo& samplingInfo );
      /**
       * generate (@see IGenerator)
       */
      RawPcmData::Ptr generate( size_t length );
};

} /// namespace Synthesizer

#endif // SAWTOOTHGENERATOR_H
