#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

#include "IGenerator.h"

#include "TRandom3.h"

namespace Synthesizer {

class NoiseGenerator : public IGenerator
{
   public:
      NoiseGenerator( const SamplingInfo& samplingInfo, int seed = -1 );
      RawPcmData::Ptr generate( size_t length );

   private:
      TRandom3    m_ranGen;
      static int  s_ranSeed;
};

} /// namespace Synthesizer

#endif // NOISEGENERATOR_H
