#include "PeakSustainAlgorithm.h"

#include "IndexPair.h"
#include "Logger.h"

#include <cassert>
#include <cmath>
#include <limits>
#include <map>

/// Anonymous namespace.
namespace
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// connectPeaks
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector< Feature::SustainedPeak* > connectPeaks( const std::vector< Feature::SustainedPeak* >& sustPeaks, const std::vector< Feature::IBasicSpectrumPeak* >& peaks, std::vector< Feature::IBasicSpectrumPeak* >& unconnectedPeaks )
{
   assert( unconnectedPeaks.size() == 0 );

   std::map< size_t, std::map< double, size_t > > peakConnections;

   std::vector< Feature::SustainedPeak* > currSustPeaks;

   for ( size_t iSustPeak = 0; iSustPeak < sustPeaks.size(); ++iSustPeak )
   {
      double minDist = std::numeric_limits< double >::max();
      size_t minDistIndex = peaks.size();

      double freqSust = sustPeaks[ iSustPeak ]->getAllPeaks().back()->getFrequency();

      for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
      {
         double dist = fabs( freqSust - peaks[ iPeak ]->getFrequency() );
         if ( dist < minDist )
         {
            minDist = dist;
            minDistIndex = iPeak;
         }
      }

      if ( minDistIndex < peaks.size() )
      {
         peakConnections[ minDistIndex ].insert( std::pair< double, size_t >( minDist, iSustPeak ) );
      }
   }

   for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
   {
      if ( peakConnections[ iPeak ].size() == 0 )
      {
         unconnectedPeaks.push_back( peaks[ iPeak ] );
      }
      else
      {
         /// TODO: Make use of frequency uncertainty to replace hard coded association distance of 5 Hz.
         if ( peakConnections[ iPeak ].begin()->first < 5 )
         {
            size_t sustIndex = peakConnections[ iPeak ].begin()->second;
            sustPeaks[ sustIndex ]->connectPeak( peaks[ iPeak ] );
            currSustPeaks.push_back( sustPeaks[ sustIndex ] );
         }
         else
         {
            unconnectedPeaks.push_back( peaks[ iPeak ] );
         }
      }
   }

   return currSustPeaks;
}


} /// Anonymous namespace

namespace Feature
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SustainedPeak implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SustainedPeak::SustainedPeak( const IBasicSpectrumPeak* firstPeak ) :
   m_connectedPeaks( 1, firstPeak )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// connectPeak
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SustainedPeak::connectPeak( const IBasicSpectrumPeak* nextPeak )
{
   m_connectedPeaks.push_back( nextPeak );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getStartTimeSamples
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t SustainedPeak::getStartTimeSamples() const
{
   return m_connectedPeaks.front()->getStartTimeSamples();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getEndTimeSamples
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t SustainedPeak::getEndTimeSamples() const
{
   return m_connectedPeaks.back()->getEndTimeSamples();
}

} //// namespace Feature

namespace FeatureAlgorithm
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PeakSustainAlgorithm implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PeakSustainAlgorithm::PeakSustainAlgorithm( const std::string& name, const AlgorithmBase* parent ) :
   AlgorithmBase( name, parent )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// execute
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector< Feature::SustainedPeak* > PeakSustainAlgorithm::execute( const std::vector< std::vector< Feature::IBasicSpectrumPeak* > >& peaks )
{
   getLogger() << Msg::Verbose << "Number of hops: " << peaks.size() << Msg::EndReq;

   assert( peaks.size() > 0 );

   std::vector< Feature::SustainedPeak* > result;

   std::vector< Feature::SustainedPeak* > currentSustainedPeaks;
   currentSustainedPeaks.reserve( peaks[ 0 ].size() );

   for ( size_t iPeak = 0; iPeak < peaks[ 0 ].size(); ++iPeak )
   {
      getLogger() << Msg::Verbose << "Init: Adding sustained peak with frequency " << peaks[ 0 ][ iPeak ]->getFrequency() << Msg::EndReq;
      currentSustainedPeaks.push_back( new Feature::SustainedPeak( peaks[ 0 ][ iPeak ] ) );
   }
   result = currentSustainedPeaks;

   for ( size_t iHop = 0; iHop < peaks.size() - 1; ++iHop )
   {
      std::vector< Feature::IBasicSpectrumPeak* > unconnectedPeaks;

      getLogger() << Msg::Verbose << "currentSustainedPeaks.size() = " << currentSustainedPeaks.size() << Msg::EndReq;
      currentSustainedPeaks = connectPeaks( currentSustainedPeaks, peaks[ iHop + 1 ], unconnectedPeaks );

      for ( size_t iUnconnectedPeak = 0; iUnconnectedPeak < unconnectedPeaks.size(); ++ iUnconnectedPeak )
      {
         currentSustainedPeaks.push_back( new Feature::SustainedPeak( unconnectedPeaks[ iUnconnectedPeak ] ) );
         result.push_back( currentSustainedPeaks.back() );
      }
   }

   return result;
}

} /// namespace FeatureAlgorithm
