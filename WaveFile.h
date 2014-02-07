#ifndef WAVEFILE_H
#define WAVEFILE_H

#include <string>
#include <fstream>

class MultiChannelRawPcmData;

/**
 * @class WaveFile
 * @brief Container of methods that can read/write waveFiles from SoundData
 *
 * The read and write methods can throw exceptions, so these should always be encompassed by a try-catch block
 */
class WaveFile
{
   public:
      /**
       * Read a wavefile with name @param fileName.
       * The ownership of SoundData is transferred to caller.
       */
      static MultiChannelRawPcmData* read( const std::string& fileName );

      /**
       * Write a wavefile with name @param fileName from @param soundData.
       */
      static void write( const std::string& fileName, const MultiChannelRawPcmData& soundData );
};

#endif // WAVEFILE_H
