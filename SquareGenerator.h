#ifndef SQUAREGENERATOR_H
#define SQUAREGENERATOR_H

#include "IGenerator.h"

namespace Synthesizer {

class SquareGenerator : public IGenerator
{
   public:
      RawPcmData::Ptr generate( size_t length, const SamplingInfo& samplingInfo );
};

} /// namespace Synthesizer

#endif // SQUAREGENERATOR_H
