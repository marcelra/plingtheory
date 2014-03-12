#ifndef SAWTOOTHGENERATOR_H
#define SAWTOOTHGENERATOR_H

#include "IGenerator.h"

namespace Synthesizer
{

/// TODO: doc
class SawtoothGenerator : public IGenerator
{
   public:
      SawtoothGenerator( const SamplingInfo& samplingInfo );
      RawPcmData::Ptr generate( size_t length );
};

} /// namespace Synthesizer

#endif // SAWTOOTHGENERATOR_H
