#include "AccumArrayPeakAlgorithm.h"

#include "RootUtilities.h"
#include "SampledMovingAverage.h"

#include "TH1F.h"

#include <cmath>

#include <iostream>

namespace FeatureAlgorithm
{

AccumArrayPeakAlgorithm::AccumArrayPeakAlgorithm()
{
   m_smoothFraction = 1 / 100.;
   m_sigmaFactor = 1;
}

std::vector< Feature::Peak > AccumArrayPeakAlgorithm::execute( const Math::RegularAccumArray& data )
{
   std::vector< Feature::Peak > result;

   const RealVector& smoothedData = calculateSmoothedData( data );

//    /// Temp: put it in accum array to fill hist
//    assert( smoothedData.size() == data.getNumBins() );
//    Math::RegularAccumArray smoothAa( data.getNumBins(), data.getMinX(), data.getMaxX() );
//
//    for ( size_t iBin = 0; iBin < smoothAa.getNumBins(); ++iBin )
//    {
//       smoothAa.setBinContent( iBin, smoothedData[ iBin ] );
//    }
//
//    new TCanvas();
//    TH1F* histData = data.createHistogram();
//    TH1F* histSmoothed = smoothAa.createHistogram();

//    histData->Draw();
//    histSmoothed->SetLineColor( kBlue );
//    histSmoothed->Draw( "SAME" );
   const RealVector& dataContents = data.getAllBinContents();
   TGraph* grData = RootUtilities::createGraph( dataContents );
   TGraph* grSmooth = RootUtilities::createGraph( smoothedData );

   grData->Draw( "AL" );
   grData->SetLineColor( kGray + 2 );
   grSmooth->Draw( "LSAME" );
   grSmooth->SetLineColor( kBlue );

   const RealVector& baselineSubtractedData = subtractBaseline( smoothedData, dataContents );
   // RealVector baselineSubtractedData( smoothedData.size() );
   // for ( size_t i = 0; i < smoothedData.size(); ++i )
   // {
   //    baselineSubtractedData[ i ] = dataContents[ i ] > baselineSubtractedData[ i ] ? dataContents[ i ] - smoothedData[ i ] : 0;
   // }

   TGraph* grSub = RootUtilities::createGraph( baselineSubtractedData );
   grSub->Draw( "LSAME" );

   return result;
}

RealVector AccumArrayPeakAlgorithm::subtractBaseline( const RealVector& smoothedData, const RealVector& originalData ) const
{
   RealVector subtractedData( smoothedData.size() );

   std::vector< double > minPos;
   RealVector minima;

   minima.push_back( smoothedData[ 0 ] );
   minPos.push_back( 0 );

   double leftDeriv = smoothedData[ 1 ] - smoothedData[ 0 ];

   for ( size_t i = 1; i < smoothedData.size() - 2; ++i )
   {
      double rightDeriv = smoothedData[ i + 1 ] - smoothedData[ i ];

      if ( leftDeriv < 0 && rightDeriv > 0 )
      {
         minima.push_back( smoothedData[i] );
         minPos.push_back( i );
      }

      leftDeriv = rightDeriv;
   }

   minima.push_back( smoothedData[ smoothedData.size() - 1 ] );
   minPos.push_back( smoothedData.size() - 1 );

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

   TGraph* grMin = RootUtilities::createGraph( minPos, minima );
   grMin->Draw( "LSAME" );
   grMin->SetLineColor( kRed );

   return subtractedData;
}

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

} /// FeatureAlgorithm
