#include "DevSuite.h"
#include "Logger.h"
#include "TestDataSupply.h"

#include "TriangleGenerator.h"
#include "KernelPdf.h"
#include "SpectralReassignmentTransform.h"
#include "MonophonicSimpleRandomMusicGenerator.h"
#include "IPlotFactory.h"
#include "PredefinedRealFunctions.h"
#include "GaussPdf.h"
#include "SortCache.h"
#include "SineGenerator.h"
#include "SampledMovingAverage.h"
#include "LinearInterpolator.h"
#include "Utils.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// execute
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::execute()
{
   Logger msg( "DevSuite" );
   msg << Msg::Info << "Running development code..." << Msg::EndReq;

   devPeakFinder();

   return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devPeakFinder
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::devPeakFinder()
{
   Logger msg( "devPeakFinder" );
   msg << Msg::Info << "Running devPeakFinder..." << Msg::EndReq;

   size_t fourierSize = 1024;
   double cutoff = 0.25;

   SamplingInfo samplingInfo( 44100 );

   RealVector inputFreqs = realVector( 100, 400, 800, 1000 );

   for ( RealVector::iterator it = inputFreqs.begin(); it != inputFreqs.end(); ++it )
   {
      Synthesizer::SineGenerator generator( samplingInfo );
      generator.setAmplitude( 0.25 );
      generator.setFrequency( *it );

      RawPcmData::Ptr data = generator.generate( fourierSize );
      WaveAnalysis::SpectralReassignmentTransform transform( samplingInfo, fourierSize, 0, 1 );

      WaveAnalysis::StftData::Ptr stftData = transform.execute( *data );
      const WaveAnalysis::SrSpectrum& spectrum = stftData->getSrSpectrum( 0 );

      msg << Msg::Info << "Number of fourier spectra = " << stftData->getNumSpectra() << Msg::EndReq;

      Math::Log10Function logFct;

      gPlotFactory().createPlot( "devPeakFinder/spectrum" );

      const RealVector& specMag = spectrum.getMagnitude();
      RealVector specMagNormalised = specMag;
      normalise( specMagNormalised );

      gPlotFactory().createScatter( spectrum.getFrequencies(), logFct.evalMany( specMag ) );

      double fourierBinSize = stftData->getConfig().getFrequencyBinWidth();
      Math::KernelPdf kernelPdf( Math::IPdf::CPtr( new Math::GaussPdf( 0, fourierBinSize ) ), spectrum.getFrequencies(), spectrum.getMagnitude() );

      Math::IRealFunction::Ptr pdfFct = kernelPdf.getDensityAsFunc();
      const RealVector& xEval = stftData->getConfig().getSpectrumFrequencies();

      // gPlotFactory().createGraph( xEval, logFct.evalMany( pdfFct->evalMany( xEval ) ) );

      const RealVector& frequencies = spectrum.getFrequencies();
      SortCache freqSort( frequencies );
      const RealVector& freqSorted = freqSort.applyTo( frequencies );

      RealVector distVec( freqSorted.size() - 1 );
      std::vector< size_t > peakCandidatePoints;
      std::vector< size_t > bkgPoints;

      size_t nBins = 1000;
      double distMin = 0;
      double distMax = -1;
      double prevDist = 1000;
      for ( size_t i = 0; i < freqSorted.size() - 1; ++i )
      {
         double dist = fabs( freqSorted[ i ] - freqSorted[ i + 1 ] ) / fourierBinSize;
         distVec[ i ] = dist;

         if ( dist < cutoff || prevDist < cutoff )
         {
            peakCandidatePoints.push_back( i );
         }
         else
         {
            bkgPoints.push_back( i );
         }

         if ( dist > distMax )
         {
            distMax = dist;
         }
         prevDist = dist;
      }
      distMax += 0.1*distMax;

      const RealVector& magSorted = freqSort.applyTo( spectrum.getMagnitude() );

      RealVector selectedFrequencies( peakCandidatePoints.size() );
      RealVector selectedMags( peakCandidatePoints.size() );
      for ( size_t i = 0; i < peakCandidatePoints.size(); ++i )
      {
         selectedFrequencies[ i ] = freqSorted[ peakCandidatePoints[ i ] ];
         selectedMags[ i ] = magSorted[ peakCandidatePoints[ i ] ];
      }

      RealVector inverseSelectionFreq( bkgPoints.size() );
      RealVector inverseSelectionMag( bkgPoints.size() );

      for ( size_t i = 0; i < bkgPoints.size(); ++i )
      {
         inverseSelectionFreq[ i ] = freqSorted[ bkgPoints[ i ] ];
         inverseSelectionMag[ i ] = magSorted[ bkgPoints[ i ] ];
      }

      gPlotFactory().createScatter( selectedFrequencies, logFct.evalMany( selectedMags ), Plotting::MarkerDrawAttr( Qt::red, Plotting::MarkerCircle ) );
      gPlotFactory().createGraph( spectrum.getConfig().getSpectrumFrequencies(), logFct.evalMany( spectrum.getMagnitude() ), Qt::blue );

      Math::SampledMovingAverage movAvgCalc( Math::SampledMovingAverage::createGaussianFilter( 21, 10 ) );
      const RealVector& movAvg = movAvgCalc.calculate( inverseSelectionMag );
      gPlotFactory().createGraph( inverseSelectionFreq, logFct.evalMany( movAvg ), Qt::yellow );

      Math::LinearInterpolator movAvgInt( inverseSelectionFreq, movAvg );

      const RealVector& xEvalMovAvgInt = Utils::createRangeReal( 0, samplingInfo.getNyquistFrequency(), 1000 );
      gPlotFactory().createGraph( xEvalMovAvgInt, logFct.evalMany( movAvgInt.evalMany( xEvalMovAvgInt ) ), Qt::green );

      RealVector magAboveBaseline;
      RealVector freqAboveBaseline;

      for ( size_t i = 0; i < selectedFrequencies.size(); ++i )
      {
         if ( selectedMags[ i ] > movAvgInt( selectedFrequencies[ i ] ) )
         {
            magAboveBaseline.push_back( selectedMags[ i ] );
            freqAboveBaseline.push_back( selectedFrequencies[ i ] );
         }
      }

      gPlotFactory().createScatter( freqAboveBaseline, logFct.evalMany( magAboveBaseline ), Plotting::MarkerDrawAttr( Qt::red, Plotting::MarkerCross, 4 ) );

      Math::RegularAccumArray hist( nBins, distMin, distMax );
      for ( size_t i = 0; i < distVec.size(); ++i )
      {
         hist.add( distVec[ i ], 1 );
      }

      gPlotFactory().createPlot( "devPeakFinder/distance" );
      gPlotFactory().createHistogram( hist );

      std::vector< std::vector< size_t > > peaks;
      peaks.push_back( std::vector< size_t >() );

      for ( size_t i = 0; i < freqAboveBaseline.size(); ++i )
      {
         double dist = fabs( freqAboveBaseline[ i ] - freqAboveBaseline[ i + 1 ] ) / fourierBinSize;
         distVec[ i ] = dist;

         if ( dist < cutoff || prevDist < cutoff )
         {
            peaks.back().push_back( i );
         }
         else
         {
            peaks.push_back( std::vector< size_t >() );
         }
      }

      RealVector peakHeights( peaks.size() );
      RealVector peakFreqs( peaks.size() );
      for ( size_t i = 0; i < peaks.size(); ++i )
      {
         for ( size_t j = 0; j < peaks[ i ].size(); ++ j )
         {
            size_t index = peaks[ i ][ j ];
            peakHeights[ i ] += magAboveBaseline[ index ];
            peakFreqs[ i ] += magAboveBaseline[ index ] * freqAboveBaseline[ index ];
         }
         if ( peakHeights[ i ] > 0 )
         {
            peakFreqs[ i ] /= peakHeights[ i ];
         }
         else
         {
            peakFreqs[ i ] = -1;
         }
      }

      gPlotFactory().createPlot( "devPeakFinder/selectedPeaks" );
      gPlotFactory().createScatter( freqAboveBaseline, magAboveBaseline, Plotting::MarkerDrawAttr( Qt::red, Plotting::MarkerCross, 2 ) );
      gPlotFactory().createScatter( peakFreqs, peakHeights, Plotting::MarkerDrawAttr( Qt::blue, Plotting::MarkerCircle ) );
   }
}

