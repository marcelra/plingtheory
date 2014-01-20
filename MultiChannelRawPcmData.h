#ifndef MULTICHANNELRAWPCMDATA_H
#define MULTICHANNELRAWPCMDATA_H

#include "RawPcmData.h"

/**
 * @class MultiChannelRawPcmData
 * @brief Class that holds multiple RawPcmData (however, mono music is also possible)
 * Note that the RawPcmData objects are owned by this object
 */
class MultiChannelRawPcmData
{
   public:
      /**
       * Empty constructor
       */
      MultiChannelRawPcmData() {}
      /**
       * Create a mono MCRPD object
       */
      MultiChannelRawPcmData( RawPcmData* monoChannel );
      /**
       * Create a stereo MCRPD object
       */
      MultiChannelRawPcmData( RawPcmData* leftChannel, RawPcmData* rightChannel );
      /**
       * Destructor
       */
      ~MultiChannelRawPcmData();

      /**
       * Copy constructor
       */
      MultiChannelRawPcmData( const MultiChannelRawPcmData& other );

      /**
       * addChannel stores the @param channelData. The MultiChannelRawPcmData object will now take care of the ownership
       * of the RawPcmData object
       */
      void addChannel( RawPcmData* channelData );

      /**
       * getChannel (@param channelId is a simple C-index that runs from [0,getNumChannels()]
       */
      const RawPcmData& getChannel( size_t channelId ) const;
      RawPcmData& getChannel( size_t channelId );
      /**
       * Query the number of channels owned by the object
       */
      size_t getNumChannels() const;

      /**
       * Get the number of samples (calls check() in order to check for ambiguities between different channels)
       */
      size_t getNumSamples() const;

      /**
       * Get the sampling info (calls check() in order to check for ambiguities between different channels)
       */
      const SamplingInfo& getSamplingInfo() const;

      /**
       * Check whether the number of samples and the SamplingInfo objects are compatible
       */
      bool check() const;

   private:
      std::vector< RawPcmData* > m_store;

      /**
       * Assignment operator is blocked (use copy constructor instead)
       */
      MultiChannelRawPcmData& operator=( const MultiChannelRawPcmData& other );
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods MultiChannelRawPcmData
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline const RawPcmData& MultiChannelRawPcmData::getChannel( size_t channelId ) const
{
   return const_cast< MultiChannelRawPcmData* >(this)->getChannel( channelId );
}

inline RawPcmData& MultiChannelRawPcmData::getChannel( size_t channelId )
{
   return *m_store[channelId];
}

inline size_t MultiChannelRawPcmData::getNumChannels() const
{
  return m_store.size();
}

inline size_t MultiChannelRawPcmData::getNumSamples() const
{
   Logger msg( "MultiChannelRawPcmData" );
   if ( !check() )
   {
      msg << Msg::Warning << "getNumSamples: check() failed => incompatible channels => returning number of samples for first stored channel" << Msg::EndReq;
   }
   if ( m_store.size() > 0 )
   {
      return m_store[0]->size();
   }
   throw ExceptionEmptyMultiChannelRawPcmData();
}

inline const SamplingInfo& MultiChannelRawPcmData::getSamplingInfo() const
{
   Logger msg( "MultiChannelRawPcmData" );
   if ( !check() )
   {
      msg << Msg::Warning << "getSamplingInfo: check() failed => incompatible channels => return sampling info for first stored channel" << Msg::EndReq;
   }
   if ( m_store.size() > 0 )
   {
      return m_store[0]->getSamplingInfo();
   }
   throw ExceptionEmptyMultiChannelRawPcmData();
}


#endif // MULTICHANNELRAWPCMDATA_H
