#include "AccumArrayPeakAlgorithm.h"

#include "RootUtilities.h"
#include "SampledMovingAverage.h"

#include "TH1F.h"
#include "TLine.h"

#include <cmath>
#include <iostream>

namespace FeatureAlgorithm
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AccumArrayPeakAlgorithm::AccumArrayPeakAlgorithm()
{
   m_smoothFraction = 1 / 100.;
   m_sigmaFactor = 1;
   m_maxNumPeaks = 10;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// execute
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector< Feature::Peak > AccumArrayPeakAlgorithm::execute( const Math::RegularAccumArray& data ) const
{
   const RealVector& smoothedData = calculateSmoothedData( data );
   const RealVector& dataContents = data.getAllBinContents();
   const RealVector& baselineSubtractedData = subtractBaseline( smoothedData, dataContents );

   if ( m_doMonitor )
   {
      TGraph* grData = RootUtilities::createGraph( dataContents );
      grData->Draw( "AL" );
      grData->SetLineColor( kGray + 2 );
   }

   std::vector< Feature::Peak > peaks = findPeaks( baselineSubtractedData, data );
   for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
   {
   }


   if ( m_doMonitor )
   {
      // TGraph* grSmooth = RootUtilities::createGraph( smoothedData );
      // grSmooth->Draw( "LSAME" );
      // grSmooth->SetLineColor( kBlue );

      TGraph* grSub = RootUtilities::createGraph( baselineSubtractedData );
      grSub->Draw( "LSAME" );
   }
   return peaks;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// subtractBaseline
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVector AccumArrayPeakAlgorithm::subtractBaseline( const RealVector& smoothedData, const RealVector& originalData ) const
{
   /// Define result.
   RealVector subtractedData( smoothedData.size() );

   /// Find position of minima.
   const std::vector< size_t >& minPositionVec = Utils::findMinima( smoothedData );

   size_t rightPosIndex = 1;
   size_t leftPos = minPositionVec[ 0 ];
   size_t rightPos = minPositionVec[ 1 ];
   double leftMin = smoothedData[ leftPos ];
   double rightMin = smoothedData[ rightPos ];

   double step = ( rightMin - leftMin ) / ( rightPos - leftPos );
   double baseline = leftMin;

   for ( size_t i = 0; i < originalData.size(); ++i )
   {
      /// Linear interpolation, subtract if smaller than original data values.
      baseline += step;
      if ( originalData[ i ] > baseline )
      {
         subtractedData[ i ] = originalData[ i ] - baseline;
      }
      else
      {
         subtractedData[ i ] = 0;
      }

      if ( i > rightPos )
      {
         ++rightPosIndex;
         std::cout << "Right pos index = " << rightPosIndex << ", total size = " << minPositionVec.size() << std::endl;
         leftPos = rightPos;
         if ( rightPosIndex < minPositionVec.size() )
         {
            rightPos = minPositionVec[ rightPosIndex ];
         }
         else
         {
            rightPos = smoothedData.size() - 1;
         }
         leftMin = rightMin;
         rightMin = smoothedData[ rightPos ];
         step = ( rightMin - leftMin ) / ( rightPos - leftPos );
         baseline = leftMin;
      }
   }

   if ( m_doMonitor )
   {
      const RealVector& minima = Utils::getSelection( smoothedData, minPositionVec );
      RealVector minPosReal( minPositionVec.begin(), minPositionVec.end() );
      TGraph* grMin = RootUtilities::createGraph( minPosReal, minima );
      grMin->Draw( "LSAME" );
      grMin->SetLineColor( kRed );
   }

   /// Return result.
   return subtractedData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// calculateSmoothedData
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVector AccumArrayPeakAlgorithm::calculateSmoothedData( const Math::RegularAccumArray& data ) const
{
   /// Define size (in samples) of Gaussian filter.
   size_t nSamples = data.getNumBins() * m_smoothFraction;

   /// Make sure nSamples is odd.
   if ( ( nSamples % 2 ) == 0 )
   {
      ++nSamples;
   }

   Math::SampledMovingAverage movAvg( Math::SampledMovingAverage::createGaussianFilter( nSamples, nSamples * m_sigmaFactor ) );
   return movAvg.calculate( data.getAllBinContents() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setDoMonitor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccumArrayPeakAlgorithm::setDoMonitor( bool doMonitor )
{
   m_doMonitor = doMonitor;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// findPeaks
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector< Feature::Peak > AccumArrayPeakAlgorithm::findPeaks( const RealVector& baselineSubtractedData, const Math::RegularAccumArray& data ) const
{
   /// The resulting vector of peaks.
   std::vector< Feature::Peak > peaks;

   /// Between two minima there is one maximum.
   const std::vector< size_t >& minPositionVec = Utils::findMinima( baselineSubtractedData );

   /// No peaks can be found.
   if ( minPositionVec.size() < 2 )
   {
      return peaks;
   }

   /// First find the position of peaks in indices.
   for ( size_t iPeak = 0; iPeak < minPositionVec.size() - 1; ++iPeak )
   {
      size_t iLeftMin = minPositionVec[ iPeak ];
      size_t iRightMin = minPositionVec[ iPeak + 1 ];
      assert( iLeftMin < iRightMin );

      double maxVal = baselineSubtractedData[ iLeftMin ];
      size_t maxPos = iLeftMin;

      /// Find max value between two indices.
      for ( size_t iValue = iLeftMin + 1; iValue < iRightMin; ++iValue )
      {
         double val = baselineSubtractedData[ iValue ];
         if ( val > maxVal )
         {
            maxVal = val;
            maxPos = iValue;
         }
      }

      /// Flat regions will not yield successfull search of maximum. We can safely ignore this area with no peaks.
      if ( maxPos == iLeftMin )
      {
         continue;
      }

      Feature::Peak peak( maxPos );
      peak.setProminence( maxVal );
      peak.setBoundIndices( iLeftMin, iRightMin );
      peaks.push_back( peak );
   }

   dressPeaks( data, baselineSubtractedData, peaks );

   if ( m_doMonitor )
   {
      for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
      {
         double thisPeakPos = peaks[ iPeak ].getPositionIndex();

         TLine* l = new TLine( thisPeakPos, 0, thisPeakPos, 1e10 );
         l->SetLineWidth( 1 );
         l->SetLineColor( kGray );
         l->Draw( "SAME" );
      }
   }

   return peaks;
}

void AccumArrayPeakAlgorithm::dressPeaks( const Math::RegularAccumArray& data, const RealVector& baselineSubtractedData, std::vector< Feature::Peak >& peaks ) const
{
   for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
   {
      size_t binIndexPos = peaks[ iPeak ].getPositionIndex();
      const Math::TwoTuple& entries = data.getBinEntries( binIndexPos );
      double weightedPos = 0;
      double totalWeight = 0;
      for ( size_t iEntry = 0; iEntry < entries.getNumElements(); ++iEntry )
      {
         weightedPos += entries.getX()[ iEntry ] * entries.getY()[ iEntry ];
         totalWeight += entries.getY()[ iEntry ];
      }
      weightedPos /= totalWeight;
      std::cout << "Setting position to: "<< weightedPos << std::endl;
      peaks[ iPeak ].setPosition( weightedPos );
   }

   for ( size_t iPeak = 1; iPeak < peaks.size() - 1; ++iPeak )
   {
      peaks[ iPeak ].setLeftNeighbourPeak( &peaks[ iPeak - 1 ] );
      peaks[ iPeak ].setRightNeighbourPeak( &peaks[ iPeak + 1 ] );
   }
}


} /// FeatureAlgorithm
