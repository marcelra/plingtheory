#ifndef TESTDATASUPPLY_H
#define TESTDATASUPPLY_H

#include "Note.h"
#include "RawPcmData.h"
#include "StftData.h"
#include "RealVector.h"
#include "RegularAccumArray.h"

namespace Synthesizer
{
   class IGenerator;
}

/// TODO: doc
class TestDataSupply
{
   public:
      static RealVector createNoiseAndPeaks();
      static RealVector createNoiseAndPeaks( RealVector peakLocs, RealVector peakSigmas, RealVector peakAmpsAtMax );

      static Math::RegularAccumArray drawNoiseAndPeaks();
      static Math::RegularAccumArray drawNoiseAndPeaks( RealVector peakLocs, RealVector peakSigmas, RealVector peakAmpsAtMax );

      static RawPcmData::Ptr getCurrentTestSample();
      static WaveAnalysis::StftData::Ptr getSrFtData();

      static RawPcmData::Ptr generateRandomMelody( std::vector< Music::Note >* trueMelody = 0 );
      static RawPcmData::Ptr generateSoundData();
      static RawPcmData::Ptr generateChord( Synthesizer::IGenerator& generator, size_t numSamples, const std::vector< Music::Note >& notes );

      static RawPcmData::Ptr readSoundData();
};

#endif // TESTDATASUPPLY_H
