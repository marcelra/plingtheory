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
/// Connect peaks @param peaks from with sustained peaks @param sustPeaks. Returns all peaks that are still sustaining,
/// i.e. all the peaks from @param sustPeaks that are matched with a peak from @param peaks. All peaks from @param peaks
/// that are not associated to any of the sustained peaks are collected in output argument @param unconnectedPeaks.
///
/// @note: the result should not be cleared, no dynamic memory is allocated in calls to this method.
/// @note: The elements of @param sustPeaks will change.
/// @note: deficiency: one peak, A, might be close enough for association to two peaks. The peak closest to A is taken, even when
/// another peak, B, only matches to this closest peak and not to the other association candidate from peak A. It has to
/// be investigated how often this occurs and if it occurs, if it might be a better solution to associate the closest peak
/// of B with B, discard this candidate from peak A, and choose the other peak to associate with peak A.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector< Feature::SustainedPeak* > connectPeaks( std::vector< Feature::SustainedPeak* >& sustPeaks, const std::vector< Feature::IBasicSpectrumPeak* >& peaks, std::vector< Feature::IBasicSpectrumPeak* >& unconnectedPeaks )
{
   assert( unconnectedPeaks.size() == 0 );

   std::map< size_t, std::map< double, size_t > > peakConnections;

   std::vector< Feature::SustainedPeak* > currSustPeaks;

   for ( size_t iSustPeak = 0; iSustPeak < sustPeaks.size(); ++iSustPeak )
   {
      double minDist = std::numeric_limits< double >::max();
      size_t minDistIndex = peaks.size();

      double freqSust = sustPeaks[ iSustPeak ]->getAllPeaks().back()->getFrequency();
      double uncSust = 5 * sustPeaks[ iSustPeak ]->getAllPeaks().back()->getFrequencyUncertainty();

      for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
      {
         double dist = freqSust - peaks[ iPeak ]->getFrequency();
         double dist2 = dist * dist;
         double uncThisPeak = peaks[ iPeak ]->getFrequencyUncertainty();
         double totalUnc = uncSust * uncSust + uncThisPeak * uncThisPeak;
         dist = dist2 / totalUnc;

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
         if ( peakConnections[ iPeak ].begin()->first < 1 )
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
   m_connectedPeaks( 1, firstPeak ),
   m_meanFrequency( -1 ),
   m_meanHeight( -1 )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// connectPeak
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SustainedPeak::connectPeak( const IBasicSpectrumPeak* nextPeak )
{
   m_connectedPeaks.push_back( nextPeak );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getFrequency
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double SustainedPeak::getFrequency() const
{
   return m_meanFrequency;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getHeight
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double SustainedPeak::getHeight() const
{
   return m_meanHeight;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getFrequencyUncertainty
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double SustainedPeak::getFrequencyUncertainty() const
{
   return 0;
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// finishBuilding
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SustainedPeak::finishBuilding()
{
   m_meanFrequency = 0;
   m_meanHeight = 0;
   for ( size_t iPeak = 0; iPeak < m_connectedPeaks.size(); ++iPeak )
   {
      m_meanFrequency += m_connectedPeaks[ iPeak ]->getFrequency();
      m_meanHeight += m_connectedPeaks[ iPeak ]->getHeight();
   }
   m_meanFrequency /= m_connectedPeaks.size();
   m_meanHeight /= m_connectedPeaks.size();
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

   /// Calculate derived quantities for the sustained peaks.
   for ( size_t iSustainedPeak = 0; iSustainedPeak < result.size(); ++iSustainedPeak )
   {
      result[ iSustainedPeak ]->finishBuilding();
   }

   return result;
}

} /// namespace FeatureAlgorithm
