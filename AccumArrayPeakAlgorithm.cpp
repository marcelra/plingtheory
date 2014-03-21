#include "AccumArrayPeakAlgorithm.h"

#include "ComposedRealFuncWithDerivative.h"
#include "GaussPdf.h"
#include "IPlotFactory.h"
#include "KernelPdf.h"
#include "Logger.h"
#include "NewtonSolver1D.h"
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
AccumArrayPeakAlgorithm::AccumArrayPeakAlgorithm()
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

         // std::cout << "Right pos index = " << rightPosIndex << ", total size = " << minPositionVec.size() << std::endl;
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
   // msg.setThreshold( Msg::Always );

   for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
   {
      Feature::Peak& peak = peaks[ iPeak ];
      /// Do position weighting in peak bin.
      size_t binIndexPos = peak.getPositionIndex();
      const Math::TwoTuple& entries = data.getBinEntries( binIndexPos );
      double weightedPos = 0;
      double totalWeight = 0;
      for ( size_t iEntry = 0; iEntry < entries.getNumElements(); ++iEntry )
      {
         weightedPos += entries.getX()[ iEntry ] * entries.getY()[ iEntry ];
         totalWeight += entries.getY()[ iEntry ];
      }
      weightedPos /= totalWeight;
      peak.setPosition( weightedPos );

      /// Set peak data.
      IndexVector indexVec = Utils::createRange( peak.getLeftBoundIndex(), peak.getRightBoundIndex() );
      msg << Msg::Verbose << indexVec << Msg::EndReq;
      const RealVector& peakData = Utils::createSelection( baselineSubtractedData, indexVec );
      msg << Msg::Verbose << "peakData = " << peakData << Msg::EndReq;
      peak.setData( peakData );
      peak.setBounds( data.getBin( peak.getLeftBoundIndex() ).getMinX(), data.getBin( peak.getRightBoundIndex() ).getMaxX() );
   }

   for ( size_t iPeak = 1; iPeak < peaks.size() - 1; ++iPeak )
   {
      peaks[ iPeak ].setLeftNeighbourPeak( &peaks[ iPeak - 1 ] );
      peaks[ iPeak ].setRightNeighbourPeak( &peaks[ iPeak + 1 ] );
   }

   if ( m_doMonitor )
   {
      for ( size_t showPeak = 0; showPeak < peaks.size(); ++showPeak )
      {
         // Logger logSession( "PeakDressing" );
         // logSession.setLoggerThreshold( Msg::Error );
         // logSession.setLoggerThreshold( Msg::Info, 1 );

         const Feature::Peak& peak = peaks[ showPeak ];

         Logger msg( "PeakDressing" );
         msg << Msg::Info << "Dressing peak " << showPeak << Msg::EndReq;
         msg << Msg::Debug << "Peak centre is located at " << peak.getPosition() << Msg::EndReq;
         msg << Msg::Debug << peak.getData() << Msg::EndReq;

         IndexVector range = Utils::createRange( peak.getLeftBoundIndex(), peak.getRightBoundIndex() );
         Math::TwoTuple allEntries;
         for ( size_t i = 0; i < range.size(); ++i )
         {
            Math::TwoTuple binTuple = data.getBinEntries( range[ i ] );
            allEntries.append( binTuple );
         }

         RealVector peakCentreX( 1, peak.getPosition() );
         RealVector peakCentreY( 1, peak.getProminence() );

         std::map< double, size_t > distMap;
         double totalWeight = 0;
         for ( size_t iEntry = 0; iEntry < allEntries.getNumElements(); ++iEntry )
         {
            double dist = fabs( allEntries.getX( iEntry ) - peak.getPosition() );
            distMap[ dist ] = iEntry;
            totalWeight += allEntries.getY( iEntry );
         }

         Math::KernelPdf dataKernel( Math::IPdf::CPtr( new Math::GaussPdf( 0, 1 ) ), allEntries.getX(), allEntries.getY() );

         size_t nEvalPdf = 100;
         RealVector evalPdfVec( nEvalPdf );
         RealVector xVec( nEvalPdf );
         double x = peak.getLeftBound();
         double step = ( peak.getRightBound() - peak.getLeftBound() ) / nEvalPdf;
         for ( size_t i = 0; i < nEvalPdf; ++i )
         {
            evalPdfVec[ i ] = dataKernel.getDensity( x );
            xVec[ i ] = x;
            x+= step;
         }

         double peakMaxWidth = peak.getRightBound() - peak.getLeftBound();
         double probLeft = dataKernel.getIntegral( peak.getLeftBound() );
         double probRight = 1 - dataKernel.getIntegral( peak.getRightBound() );

         double startValueSolver = peak.getLeftBound() + peakMaxWidth / 2;

         Math::RealMemFunction< Math::KernelPdf > surface( &Math::KernelPdf::getIntegral, &dataKernel );
         Math::RealMemFunction< Math::KernelPdf > surfDeriv( &Math::KernelPdf::getDensity, &dataKernel );
         Math::ComposedRealFuncWithDerivative funcWDeriv( surface, surfDeriv );

         Math::NewtonSolver1D solveLeftWidth( funcWDeriv, m_peakWidthSurfFrac );
         Math::NewtonSolver1D solveRightWidth( funcWDeriv, 1 - m_peakWidthSurfFrac );

         if ( msg.getLoggerId() == 16 )
         {
            // solveLeftWidth.setLoggerThreshold( Msg::Verbose );
            const RealVector& xEval = Utils::createRangeReal( peak.getLeftBound(), peak.getRightBound(), 500 );
            const RealVector& yEvalInt = funcWDeriv.evalMany( xEval );
            const RealVector& yEvalDens = funcWDeriv.evalDerivMany( xEval );
            gPlotFactory().createPlot( "Prob" );
            gPlotFactory().createGraph( xEval, yEvalInt );
            gPlotFactory().createGraph( xEval, yEvalDens );
         }
         Math::NewtonSolver1D::Result resultLeft = solveLeftWidth.solve( startValueSolver, 100 );
         Math::NewtonSolver1D::Result resultRight = solveRightWidth.solve( startValueSolver, 100 );

         if ( !resultLeft.isConverged() )
         {
            msg << Msg::Warning << "Width solver did not converge!" << Msg::EndReq;
         }

         double minWidthX = resultLeft.getSolution();
         double maxWidthX = resultRight.getSolution();

         double width = maxWidthX - minWidthX;

         RealVector leftWidthMarkerX( 2, minWidthX );
         RealVector rightWidthMarkerX( 2, maxWidthX );
         RealVector leftWidthMarkerY( 2, 10 );
         leftWidthMarkerY[0] = 0;
         RealVector rightWidthMarkerY( 2, 10 );
         rightWidthMarkerY[0] = 0;

         msg << Msg::Verbose << "probLeft = " << probLeft << ", probRight = " << probRight << Msg::EndReq;
         msg << Msg::Verbose << "minWidthX = " << minWidthX << ", maxWidthX = " << maxWidthX << Msg::EndReq;

         std::ostringstream plotName;
         plotName << "AAPA/DressPeak" << showPeak;
         gPlotFactory().createPlot( plotName.str() );
         gPlotFactory().createGraph( Utils::createRangeReal( peak.getLeftBoundIndex(), peak.getRightBoundIndex() ) * data.getBinWidth(), peak.getData() );
         gPlotFactory().createScatter( allEntries.getX(), allEntries.getY() );
         gPlotFactory().createGraph( xVec, evalPdfVec );
         gPlotFactory().createScatter( peakCentreX, peakCentreY, Qt::red );
         // gPlotFactory().createGraph( leftWidthMarkerX, leftWidthMarkerY, Qt::blue );
         // gPlotFactory().createGraph( rightWidthMarkerX, rightWidthMarkerY, Qt::blue );
      }
   }

}


} /// FeatureAlgorithm
