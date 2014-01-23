#ifndef SINEGENERATOR_H
#define SINEGENERATOR_H

#include "IGenerator.h"

namespace Synthesizer {

class SineGenerator : public IGenerator
{
   public:
      SineGenerator( const SamplingInfo& samplingInfo );
      RawPcmData::Ptr generate( size_t length );
};

} /// namespace Synthesizer

#endif // SINEGENERATOR_H
