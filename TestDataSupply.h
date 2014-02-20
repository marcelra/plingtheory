#ifndef TESTDATASUPPLY_H
#define TESTDATASUPPLY_H

#include "Note.h"
#include "RawPcmData.h"
#include "RealVector.h"

namespace Synthesizer
{
   class IGenerator;
}

class TestDataSupply
{
   public:
      static RealVector createNoiseAndPeaks();
      static RealVector createNoiseAndPeaks( RealVector peakLocs, RealVector peakSigmas, RealVector peakAmpsAtMax );

      static RawPcmData::Ptr getCurrentTestSample();

      static RawPcmData::Ptr generateSoundData();
      static RawPcmData::Ptr generateChord( Synthesizer::IGenerator& generator, size_t numSamples, const std::vector< Music::Note >& notes );

      static RawPcmData::Ptr readSoundData();
};

#endif // TESTDATASUPPLY_H
