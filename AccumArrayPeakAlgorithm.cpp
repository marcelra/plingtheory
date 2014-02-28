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
/// findMinima
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::pair< std::vector< size_t >, RealVector > findMinima( const RealVector& vector )
{
   std::vector< size_t > minPos;
   RealVector minima;

   minima.push_back( vector[ 0 ] );
   minPos.push_back( 0 );

   double leftDeriv = vector[ 1 ] - vector[ 0 ];

   for ( size_t i = 1; i < vector.size() - 2; ++i )
   {
      double rightDeriv = vector[ i + 1 ] - vector[ i ];

      if ( leftDeriv < 0 && rightDeriv >= 0 )
      {
         minima.push_back( vector[i] );
         minPos.push_back( i );
      }

      leftDeriv = rightDeriv;
   }

   minima.push_back( vector[ vector.size() - 1 ] );
   minPos.push_back( vector.size() - 1 );

   return std::pair< std::vector< size_t >, RealVector >( minPos, minima );
}

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

   const std::vector< Feature::Peak >& peaks = findPeaks( baselineSubtractedData );

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
   RealVector subtractedData( smoothedData.size() );

   const std::pair< std::vector< size_t >, RealVector >& minimaPair = findMinima( smoothedData );

   const std::vector< size_t >& minPos = minimaPair.first;
   const RealVector& minima = minimaPair.second;

   size_t rightPosIndex = 1;
   double leftMin = minima[ 0 ];
   double rightMin = minima[ 1 ];
   size_t leftPos = minPos[ 0 ];
   size_t rightPos = minPos[ 1 ];
   double step = ( rightMin - leftMin ) / ( rightPos - leftPos );
   double baseline = leftMin;

   for ( size_t i = 0; i < originalData.size(); ++i )
   {
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
         leftPos = rightPos;
         rightPos = minPos[ rightPosIndex ];
         leftMin = rightMin;
         rightMin = minima[ rightPosIndex ];
         step = ( rightMin - leftMin ) / ( rightPos - leftPos );
         baseline = leftMin;
      }
   }

   if ( m_doMonitor )
   {
      RealVector minPosReal( minPos.begin(), minPos.end() );
      TGraph* grMin = RootUtilities::createGraph( minPosReal, minima );
      grMin->Draw( "LSAME" );
      grMin->SetLineColor( kRed );
   }

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
std::vector< Feature::Peak > AccumArrayPeakAlgorithm::findPeaks( const RealVector& baselineSubtractedData ) const
{
   std::vector< Feature::Peak > peaks;

   /// Between two minima there is one maximum
   const std::pair< std::vector< size_t >, RealVector >& minima = findMinima( baselineSubtractedData );
   const std::vector< size_t >& minPos = minima.first;
   const RealVector& minValues = minima.second;

   std::vector< size_t > peakPos;

   for ( size_t iPeak = 0; iPeak < minPos.size() - 1; ++iPeak )
   {
      size_t iLeftMin = minPos[ iPeak ];
      size_t iRightMin = minPos[ iPeak + 1 ];
      assert( iLeftMin < iRightMin );

      double maxVal = minValues[ iLeftMin ];
      size_t maxPos = iLeftMin;

      for ( size_t iValue = iLeftMin + 1; iValue < iRightMin; ++iValue )
      {
         double val = baselineSubtractedData[ iValue ];
         if ( val > maxVal )
         {
            maxVal = val;
            maxPos = iValue;
         }
      }

      /// TODO: make this a valid assert
      // assert( maxPos > iLeftMin );
      /// For now use workaround
      if ( maxPos == iLeftMin )
      {
         continue;
      }

      peakPos.push_back( maxPos );

      Feature::Peak peak( maxPos, maxVal, iRightMin - iLeftMin, 0 );
      peaks.push_back( peak );
   }

   if ( m_doMonitor )
   {
      RealVector vMinX( minPos.begin(), minPos.end() );
      TGraph* grMinVals = RootUtilities::createGraph( vMinX, minValues );
      grMinVals->SetLineColor( kRed );
      grMinVals->Draw( "LSAME" );

      for ( size_t iPeak = 0; iPeak < peakPos.size(); ++iPeak )
      {
         double thisPeakPos = peakPos[ iPeak ];
         TLine* l = new TLine( thisPeakPos, 0, thisPeakPos, 1e10 );
         l->SetLineWidth( 1 );
         l->SetLineColor( kGray );
         l->Draw( "SAME" );
      }
   }

   return peaks;
}

} /// FeatureAlgorithm
