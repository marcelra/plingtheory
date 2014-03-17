#include "AccumArrayPeakAlgorithm.h"

#include "IPlotFactory.h"
#include "Logger.h"
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

   std::vector< Feature::Peak > peaks = findPeaks( baselineSubtractedData, data );

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
   size_t leftPos = 0;
   size_t rightPos = minPositionVec[ 0 ];
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
      IndexVector allMinPos( 1, 0 );
      for ( size_t i = 0; i < minPositionVec.size(); ++i )
      {
         allMinPos.push_back( minPositionVec[ i ] );
      }
      allMinPos.push_back( smoothedData.size() - 1 );

      const RealVector& minima = Utils::createSelection( smoothedData, allMinPos );

      RealVector minPosReal( allMinPos.begin(), allMinPos.end() );

      gPlotFactory().createPlot( "AAPA/DataSubtraction" );
      gPlotFactory().createGraph( originalData, Qt::gray );
      gPlotFactory().createGraph( subtractedData, Qt::black );
      gPlotFactory().createGraph( minPosReal, minima, Qt::red );
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
      size_t rightBoundIndex = iRightMin < baselineSubtractedData.size() - 1? rightBoundIndex + 1 : rightBoundIndex;
      peak.setBoundIndices( iLeftMin, iRightMin + 1 );
      peaks.push_back( peak );
   }

   if ( m_doMonitor )
   {
      RealVector peakPositions( peaks.size() );
      RealVector peakHeights( peaks.size() );
      for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
      {
         double thisPeakPos = peaks[ iPeak ].getPositionIndex();
         peakPositions[ iPeak] = thisPeakPos;
         peakHeights[ iPeak ] = peaks[ iPeak ].getProminence();
      }
      gPlotFactory().createScatter( peakPositions, peakHeights, Qt::red );
   }

   dressPeaks( data, baselineSubtractedData, peaks );
   return peaks;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// dressPeaks
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccumArrayPeakAlgorithm::dressPeaks( const Math::RegularAccumArray& data, const RealVector& baselineSubtractedData, std::vector< Feature::Peak >& peaks ) const
{
   Logger msg( "AccumArrayPeakAlgorithm" );
   for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
   {
      /// Do position weighting in peak bin.
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
      peaks[ iPeak ].setPosition( weightedPos );

      /// Set peak data.
      IndexVector indexVec = Utils::createRange( peaks[ iPeak ].getLeftBoundIndex(), peaks[ iPeak ].getRightBoundIndex() );
      msg << Msg::Verbose << indexVec << Msg::EndReq;
      const RealVector& peakData = Utils::createSelection( baselineSubtractedData, indexVec );
      msg << Msg::Verbose << "peakData = " << peakData << Msg::EndReq;
      peaks[ iPeak ].setData( peakData );
   }

   for ( size_t iPeak = 1; iPeak < peaks.size() - 1; ++iPeak )
   {
      peaks[ iPeak ].setLeftNeighbourPeak( &peaks[ iPeak - 1 ] );
      peaks[ iPeak ].setRightNeighbourPeak( &peaks[ iPeak + 1 ] );
   }

   if ( m_doMonitor )
   {
      size_t showPeak = 0;
      const Feature::Peak& peak = peaks[ showPeak ];
      msg << Msg::Info << peak.getData() << Msg::EndReq;

      gPlotFactory().createPlot( "AAPA/DressPeak" );
      gPlotFactory().createGraph( Utils::createRangeReal( peak.getLeftBoundIndex(), peak.getRightBoundIndex() ) * data.getBinWidth(), peak.getData() );

      Logger msg( "AccumArrayPeakAlgorithm" );
      msg << Msg::Info << "Peak centre is located at " << peak.getPosition() << Msg::EndReq;

      IndexVector range = Utils::createRange( peak.getLeftBoundIndex(), peak.getRightBoundIndex() );
      Math::TwoTuple allEntries;
      for ( size_t i = 0; i < range.size(); ++i )
      {
         Math::TwoTuple binTuple = data.getBinEntries( range[ i ] );
         allEntries.append( binTuple );
      }

      std::map< double, size_t > distMap;
      double totalWeight = 0;
      for ( size_t iEntry = 0; iEntry < allEntries.getNumElements(); ++iEntry )
      {
         double dist = fabs( allEntries.getX( iEntry ) - peak.getPosition() );
         distMap[ dist ] = iEntry;
         totalWeight += allEntries.getY( iEntry );
      }

      double peakSurface = sum( peak.getData() );

      double widthDef = 0.68 ; // peakSurface / exp( 1. );
      double widthSeen = 0;
      std::map< double, size_t >::const_iterator it = distMap.begin();
      for ( ; it != distMap.end(); ++it )
      {
         widthSeen += allEntries.getY( it->second );
         if ( widthSeen > widthDef )
         {
            break;
         }
      }
      double widthTooMuch = widthSeen - widthDef;
      msg << Msg::Info << "widthTooMuch = " << widthTooMuch << ", widthDef = " << widthDef << Msg::EndReq;
      // --it->first

      gPlotFactory().createScatter( allEntries.getX(), allEntries.getY() );
   }

}


} /// FeatureAlgorithm
