#ifndef SINEGENERATOR_H
#define SINEGENERATOR_H

#include "IGenerator.h"

namespace Synthesizer {

class SineGenerator : public IGenerator
{
   public:
      RawPcmData::Ptr generate( size_t length, const SamplingInfo& samplingInfo );
};

} /// namespace Synthesizer

#endif // SINEGENERATOR_H
