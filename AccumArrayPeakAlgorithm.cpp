#include "AccumArrayPeakAlgorithm.h"

#include "BisectionSolver1D.h"
#include "ComposedRealFuncWithDerivative.h"
#include "GaussPdf.h"
#include "IPlotFactory.h"
#include "KernelPdf.h"
#include "Logger.h"
#include "NewtonSolver1D.h"
#include "PredefinedRealFunctions.h"
#include "RealMemFunction.h"
#include "RootUtilities.h"
#include "SampledMovingAverage.h"

#include "TH1F.h"
#include "TLine.h"

#include <cmath>
#include <iostream>
#include <sstream>

namespace FeatureAlgorithm
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AccumArrayPeakAlgorithm::AccumArrayPeakAlgorithm( const std::string& algorithmName, const AlgorithmBase* parent ) :
   AlgorithmBase( algorithmName, parent )
{
   m_smoothFraction = 1 / 100.;
   m_sigmaFactor = 1;
   m_maxNumPeaks = 10;
   m_peakWidthSurfFrac = 0.1;
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

      Math::Log10Function logFunc( 1e-3 );
      gPlotFactory().createPlot( "AAPA/DataSubtraction" );
      gPlotFactory().createGraph( logFunc.evalMany( originalData ), Qt::gray );
      gPlotFactory().createGraph( logFunc.evalMany( subtractedData ), Qt::black );
      gPlotFactory().createGraph( logFunc.evalMany( smoothedData ), Qt::green );
      gPlotFactory().createGraph( minPosReal, logFunc.evalMany( minima ), Qt::red );
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
   const std::vector< size_t >& minPositionVecRaw = Utils::findMinima( baselineSubtractedData );

   /// TODO: find more elegant solution for this.
   std::vector< size_t > minPositionVec;
   minPositionVec.push_back( 0 );
   for ( size_t i = 0; i < minPositionVecRaw.size(); ++i )
   {
      minPositionVec.push_back( minPositionVecRaw[ i ] );
   }
   minPositionVec.push_back( baselineSubtractedData.size() - 1 );

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
      size_t rightBoundIndex = iRightMin < baselineSubtractedData.size() - 1? iRightMin + 1 : iRightMin;
      peak.setBoundIndices( iLeftMin, rightBoundIndex );
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
      Math::Log10Function logFunc( 1e-3 );
      gPlotFactory().createScatter( peakPositions, logFunc.evalMany( peakHeights ), Plotting::MarkerDrawAttr( Qt::red ) );
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
   // msg.setThreshold( Msg::Always );

   std::vector< Math::IPdf* > kernPdfs;

   /// Peak entries
   /// Total weight (corrected, uncorrected)

   /// Peak neighbours
   /// Peak data
   for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
   {
      Feature::Peak& peak = peaks[ iPeak ];

      if ( iPeak != 0 )
      {
         peak.setLeftNeighbourPeak( &peaks[ iPeak ] );
      }
      else
      {
         peak.setLeftNeighbourPeak( 0 );
      }

      if ( iPeak != peaks.size() - 1 )
      {
         peak.setRightNeighbourPeak( &peaks[ iPeak ] );
      }
      else
      {
         peak.setRightNeighbourPeak( 0 );
      }

      RealVector peakData;
      size_t iBinLeft = peak.getLeftBoundIndex();
      size_t iBinRight = peak.getRightBoundIndex();
      for ( size_t iBin = iBinLeft; iBin < iBinRight; ++iBin )
      {
         peakData.push_back( baselineSubtractedData[ iBin ] );
      }

      peak.setData( peakData );

      double xLeft = data.getBin( iBinLeft ).getMinX();
      double xRight = data.getBin( iBinRight ).getMaxX();

      peak.setBounds( xLeft, xRight );

   }

   /// Centre of mass
   for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
   {
      Feature::Peak& peak = peaks[ iPeak ];

      double peakCm = 0;
      double peakWeight = 0;

      size_t iBinLeft = peak.getLeftBoundIndex();
      size_t iBinRight = peak.getRightBoundIndex();

      for ( size_t iBin = iBinLeft; iBin <= iBinRight; ++iBin )
      {
         const Math::TwoTuple& entries = data.getBinEntries( iBin );
         for ( size_t iEntry = 0; iEntry < entries.getNumElements(); ++iEntry )
         {
            peakCm += entries.getX( iEntry ) * entries.getY( iEntry );
            peakWeight += entries.getY( iEntry );
         }
      }

      peakCm /= peakWeight;
      peak.setPosition( peakCm );
   }

   /// Width
   for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
   {
      Feature::Peak& peak = peaks[ iPeak ];

      Math::TwoTuple allPeakEntries;

      for ( size_t iBin = peak.getLeftBoundIndex(); iBin < peak.getRightBoundIndex(); ++iBin )
      {
          const Math::TwoTuple& binEntries = data.getBinEntries( iBin );
          allPeakEntries.append( binEntries );
      }
      peak.setPeakEntries( allPeakEntries );

      Math::KernelPdf* kernPdf = new Math::KernelPdf( Math::IPdf::CPtr( new Math::GaussPdf( 0, 1 ) ), allPeakEntries.getX(), allPeakEntries.getY() );
      kernPdfs.push_back( kernPdf );
      Math::RealMemFunction< Math::KernelPdf > memFunc( &Math::KernelPdf::getIntegral, kernPdf );
      Math::IRealFunction& func = memFunc;

      Interval solutionInterval( peak.getLeftBound(), peak.getRightBound() );

      Math::BisectionSolver1D solver;
      Math::BisectionSolver1D::Result resultLeft = solver.solve( func, m_peakWidthSurfFrac, solutionInterval );
      Math::BisectionSolver1D::Result resultRight = solver.solve( func, 1 - m_peakWidthSurfFrac, solutionInterval );
      if ( resultLeft.isConverged() && resultRight.isConverged() )
      {
         peak.setWidth( ( resultRight.getSolution() - resultLeft.getSolution() ) / 2. );
      }
      else
      {
         getLogger() << Msg::Debug << "Width determination failed!" << Msg::EndReq;
      }
   }


   /// Monitoring code
   if ( m_doMonitor )
   {
      for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
      {
         Feature::Peak& peak = peaks[ iPeak ];

         const RealVector& peakEntries = Utils::createRangeReal( peak.getLeftBoundIndex(), peak.getRightBoundIndex() );
         const RealVector& peakData = peak.getData();

         assert( peakData.size() == peakEntries.size() );


         std::ostringstream peakName;
         peakName << "Peak_" << iPeak;
         gPlotFactory().createPlot( peakName.str() );
         gPlotFactory().createScatter( peak.getPeakEntries()->getX(), peak.getPeakEntries()->getY() );
         gPlotFactory().createGraph( peakEntries * data.getBinWidth(), peakData, Qt::black );

         const RealVector& xEvalPdf = Utils::createRangeReal( peak.getLeftBound(), peak.getRightBound(), 200 );
         const RealVector& yEvalPdf = kernPdfs[ iPeak ]->getDensityAsFunc()->evalMany( xEvalPdf );
         gPlotFactory().createGraph( xEvalPdf, yEvalPdf, Qt::blue );


         RealVector xCentre( 2, peak.getPosition() );
         RealVector yCentre( 1, peak.getProminence() );
         yCentre.push_back( 0 );

         gPlotFactory().createGraph( xCentre, yCentre, Qt::blue );

         RealVector xWidth;
         xWidth.push_back( peak.getPosition() - 0.5 * peak.getWidth() );
         xWidth.push_back( peak.getPosition() + 0.5 * peak.getWidth() );
         RealVector yWidth( 2, 0.5 * peak.getProminence() );

         gPlotFactory().createGraph( xWidth, yWidth, Qt::red );
      }
   }

   for ( size_t iKernPdf = 0; iKernPdf < kernPdfs.size(); ++iKernPdf )
   {
      delete kernPdfs[ iKernPdf ];
   }
}


} /// FeatureAlgorithm
