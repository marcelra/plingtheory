#include "WaveFile.h"

#include "BinaryUtilities.h"
#include "Exceptions.h"
#include "GlobalLogParameters.h"
#include "Logger.h"
#include "MultiChannelRawPcmData.h"

////////////////////////////////////////////////////////////////////////////////
/// WaveFile::read
////////////////////////////////////////////////////////////////////////////////
MultiChannelRawPcmData* WaveFile::read( const std::string& fileName )
{
   /// Open file
   std::ifstream fStream( fileName.c_str(), std::fstream::in | std::fstream::binary );

   /// Check if file is open
   if ( !fStream )
   {
      gLog() << Msg::Warning << "Could not open file " << fileName << Msg::EndReq;
      throw ExceptionFileNotFound( fileName );
   }

   gLog() << Msg::Debug << "Reading file " << fileName << Msg::EndReq;

   /// Define buffer to hold the header
   char hdrBuffer[44];

   /// Read complete header
   fStream.seekg( 0 );
   fStream.read( hdrBuffer, 44 );

   /// Check if the header has been read
   if ( !fStream )
   {
      gLog() << Msg::Warning << "Wave file " << fileName << " not found." << Msg::EndReq;
      throw ExceptionRead( fileName, "Could not open." );
   }

   /// Parse and check wave file header
   bool ok = true;
   ok &= BinaryUtilities::compare( hdrBuffer, 0, "RIFF", 4 );
   ok &= BinaryUtilities::compare( hdrBuffer, 8, "WAVE", 4 );
   ok &= BinaryUtilities::compare( hdrBuffer, 12, "fmt ", 4 );
   ok &= BinaryUtilities::compare( hdrBuffer, 36, "data", 4 );

   /// Read some quantities that will be checked later
   short audioFormat   = BinaryUtilities::readShort( hdrBuffer, 20 );
   short numChannels   = BinaryUtilities::readShort( hdrBuffer, 22 );
   int   sampleRate    = BinaryUtilities::readInt  ( hdrBuffer, 24 );
   short bitsPerSample = BinaryUtilities::readShort( hdrBuffer, 34 );
   int   subChunk2Size = BinaryUtilities::readInt  ( hdrBuffer, 40 );

   /// Not used yet
   // int   chunkSize     = BinaryUtilities::readInt  ( hdrBuffer, 4  );
   // int   subChunk1Size = BinaryUtilities::readInt  ( hdrBuffer, 16 );
   // int   byteRate      = BinaryUtilities::readInt  ( hdrBuffer, 28 );
   // short blockAlign    = BinaryUtilities::readShort( hdrBuffer, 32 );

   /// Not all WAVE-file types are supported: additional checks.
   bool isSupported = true;
   isSupported &= audioFormat == 1;
   isSupported &= numChannels <= 2;
   isSupported &= bitsPerSample <= 16;

   /// Throw exception if checks failed.
   if ( !ok )
   {
      throw ExceptionRead( fileName, "Not a valid wave file." );
   }
   /// Throw exception if wave file format is not supported.
   if ( !isSupported )
   {
      throw ExceptionRead( fileName, "Unsupported format." );
   }

   /// Calculate derived quantities
   size_t bytesPerSample = bitsPerSample / 8;
   size_t numSamples = subChunk2Size / bytesPerSample / numChannels;

   /// Set SamplingInfo
   SamplingInfo samplingInfo;
   samplingInfo.setSamplingRate( sampleRate );

   /// Initialise MultiChannelRawPcmData object
   MultiChannelRawPcmData* data = new MultiChannelRawPcmData();
   for ( short i = 0; i < numChannels; ++i )
   {
      RawPcmData* channel = new RawPcmData( samplingInfo, numSamples );
      data->addChannel( channel );
   }

   /// Set reading position directly after buffer
   fStream.seekg( 44 );

   /// Define reading buffer
   const size_t bufSize = 1024*bytesPerSample*numChannels;
   char dataBuffer[bufSize];
   size_t bufPos = bufSize;

   gLog() << Msg::Debug << "Reading " << numSamples << " samples (" << numSamples/sampleRate/60. << " minutes of music)" << Msg::EndReq;

   /// Read PCM chunk
   for ( size_t iSample = 0; iSample < numSamples; ++iSample )
   {
      /// Fill new buffer (if needed)
      if ( bufPos >= bufSize )
      {
         fStream.seekg( 44 + bytesPerSample*numChannels*iSample );
         fStream.read( dataBuffer, bufSize );
         bufPos = 0;
      }

      /// Read a sample for all channels
      for ( size_t iChannel = 0; iChannel < data->getNumChannels(); ++iChannel )
      {
         short sample = BinaryUtilities::readShort( dataBuffer, bufPos );
         data->getChannel( iChannel ).setUnnormalisedSample(iSample, sample);
         bufPos += bytesPerSample;
      }
   } /// Loop over PCM samples

   /// Report finished
   gLog() << Msg::Debug << "Done." << Msg::EndReq;

   /// Return
   return data;

}

////////////////////////////////////////////////////////////////////////////////
/// WaveFile::write
////////////////////////////////////////////////////////////////////////////////
void WaveFile::write( const std::string& fileName, const MultiChannelRawPcmData& soundData )
{
   /// Open output stream
   std::ofstream fStream( fileName.c_str(), std::fstream::out | std::fstream::binary | std::fstream::trunc );

   /// Throw exception if file cannot be opened
   if ( !fStream )
   {
      throw ExceptionFileNotFound( fileName );
   }

   /// Get properties of the sound data
   size_t numSamples    = soundData.getNumSamples();
   size_t numChannels   = soundData.getNumChannels();
   size_t sampleRate    = soundData.getSamplingInfo().getSamplingRate();

   /// Bits per sample is always 16 (converted from double)
   size_t bitsPerSample = 16;

   /// Calculate derived quantities (to be stored in WaveFile header)
   size_t subChunk2Size = numSamples * numChannels * bitsPerSample / 8;
   short blockAlign = numChannels * bitsPerSample / 8;
   size_t byteRate = sampleRate * numChannels * bitsPerSample / 8;

   /// Define header buffer
   char* hdrBuffer = new char[44];
   BinaryUtilities::writeCString( hdrBuffer, 0, "RIFF" );
   BinaryUtilities::writeInt( hdrBuffer, 4, subChunk2Size + 36 );
   BinaryUtilities::writeCString( hdrBuffer, 8, "WAVE" );
   BinaryUtilities::writeCString( hdrBuffer, 12, "fmt " );
   BinaryUtilities::writeInt( hdrBuffer, 16, 16 );
   BinaryUtilities::writeShort( hdrBuffer, 20, 1 );
   BinaryUtilities::writeShort( hdrBuffer, 22, numChannels );
   BinaryUtilities::writeInt( hdrBuffer, 24, sampleRate );
   BinaryUtilities::writeInt( hdrBuffer, 28, byteRate );
   BinaryUtilities::writeShort( hdrBuffer, 32, blockAlign );
   BinaryUtilities::writeShort( hdrBuffer, 34, bitsPerSample );
   BinaryUtilities::writeCString( hdrBuffer, 36, "data" );
   BinaryUtilities::writeInt( hdrBuffer, 40, subChunk2Size );

   gLog() << Msg::Info << "Writing wave file " << fileName << "..." << Msg::EndReq;

   /// Write header
   fStream.write( hdrBuffer, 44 );

   /// Clean up buffer
   delete hdrBuffer;

   /// Define PCM write-out buffer
   size_t nSamplesBuf = 4096;
   size_t bufLen = nSamplesBuf*numChannels*bitsPerSample/8;
   char buf[bufLen];
   char* bufPos = buf;
   char* bufPosEnd = buf + bufLen;

   /// Loop over samples
   for ( size_t i = 0; i < numSamples; ++i )
   {
      /// Loop over channels (write a sample per channel to buffer)
      for ( size_t iChannel = 0; iChannel < numChannels; ++iChannel )
      {
         short sample = soundData.getChannel( iChannel ).getUnnormalisedSample(i);
         BinaryUtilities::writeShort( bufPos, 0, sample );
         bufPos += 2;
      }

      /// Flush buffer when full
      if ( bufPos == bufPosEnd)
      {
         fStream.write( buf, bufLen );
         bufPos = buf;
      }

   } /// Loop over samples

   ///  Write tail of buffer not yet flushed at last buffer filling iteration
   fStream.write( buf, bufPos - buf );

   /// Close file
   fStream.close();

   /// Report
   gLog() << Msg::Info << "Writing wave file complete." << Msg::EndReq;
}
