#ifndef SQUAREGENERATOR_H
#define SQUAREGENERATOR_H

#include "IGenerator.h"

namespace Synthesizer {

class SquareGenerator : public IGenerator
{
   public:
      SquareGenerator( const SamplingInfo& samplingInfo );
      RawPcmData::Ptr generate( size_t length );
};

} /// namespace Synthesizer

#endif // SQUAREGENERATOR_H
