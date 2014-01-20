#include "MultiChannelRawPcmData.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor (mono sounds)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MultiChannelRawPcmData::MultiChannelRawPcmData( RawPcmData* monoChannel )
{
   m_store.push_back( monoChannel );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor (stereo sounds)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MultiChannelRawPcmData::MultiChannelRawPcmData( RawPcmData* leftChannel, RawPcmData* rightChannel )
{
   m_store.push_back( leftChannel );
   m_store.push_back( rightChannel );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// addChannel
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MultiChannelRawPcmData::addChannel( RawPcmData* channelData )
{
   m_store.push_back( channelData );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MultiChannelRawPcmData::~MultiChannelRawPcmData()
{
   /// Clean-up store
   for ( std::vector< RawPcmData* >::iterator it = m_store.begin(); it != m_store.end(); ++it )
   {
      delete *it;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Copy constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MultiChannelRawPcmData::MultiChannelRawPcmData( const MultiChannelRawPcmData& other )
{
   for ( std::vector< RawPcmData* >::const_iterator it = other.m_store.begin(); it != other.m_store.end(); ++it )
   {
      m_store.push_back( new RawPcmData( **it ) );
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// check
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MultiChannelRawPcmData::check() const
{
   Logger msg("MultiChannelRawPcmData");

   /// Return false if there are no channels
   if ( m_store.size() == 0 )
   {
      msg << Msg::Verbose << "check() fails because there are no channels" << Msg::EndReq;
      return false;
   }

   if ( m_store.size() == 1 )
      return true;

   bool checkFlag = true;
   for ( size_t iChannel = 1; iChannel < getNumChannels(); ++iChannel )
   {
      checkFlag &= (m_store[iChannel-1]->getSamplingInfo() == m_store[iChannel]->getSamplingInfo());
      if ( !checkFlag )
      {
         msg << Msg::Verbose << "check() fails on incompatible sample rate of channel " << iChannel-1 << " and channel " << iChannel << Msg::EndReq;
         break;
      }
      checkFlag &= (m_store[iChannel-1]->size() == m_store[iChannel]->size());
      if ( !checkFlag )
      {
         msg << Msg::Verbose << "check() fails on incompatible number of samples between channel " << iChannel-1 << " and channel " << iChannel << Msg::EndReq;
         msg << Msg::Verbose << "Number of samples of channel " << iChannel-1 << " is " << m_store[iChannel-1]->size()
                             << "while number of samples of channel " << iChannel << " is " << m_store[iChannel]->size() << Msg::EndReq;
         break;
      }
   }
   return checkFlag;
}

