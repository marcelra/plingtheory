#include "DevGui.h"
#include "DevSuite.h"
#include "IPlotFactory.h"
#include "Logger.h"
#include "TestDataSupply.h"
#include "Utils.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// execute
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::execute()
{
   Logger msg( "DevSuite" );
   msg << Msg::Info << "Running development code..." << Msg::EndReq;

   // devIterateSrPeaks();

   // devPeakSustainAlgorithm();

   // devFundamentalFreqFinder();
   // devTimeStretcher();

   devImprovedPeakAlgorithm();

   return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Include section
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MultiChannelRawPcmData.h"
#include "PeakSustainAlgorithm.h"
#include "RebinnedSRGraph.h"
#include "SpectralReassignmentTransform.h"
#include "SrSpecPeakAlgorithm.h"
#include "RandomNumberGenerator.h"
#include "ApproximateGcdAlgorithm.h"
#include "TimeStretcher.h"
#include "WaveFile.h"
#include "SineGenerator.h"
#include "PredefinedRealFunctions.h"
#include "KernelPdf.h"
#include "GaussPdf.h"
#include "LinearInterpolator.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devIterateSrPeaks
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::devIterateSrPeaks()
{
   Logger msg( "devIterateSrPeaks" );
   msg << Msg::Info << "Running devIterateSrPeaks..." << Msg::EndReq;

   std::unique_ptr< std::vector< Music::Note > > trueMelody( new std::vector< Music::Note >() );
   RawPcmData::Ptr data = TestDataSupply::generateRandomMelody( trueMelody.get() );
   // RawPcmData::Ptr data = TestDataSupply::getCurrentTestSample();

   size_t fourierSize = 1024;

   WaveAnalysis::SpectralReassignmentTransform transformAlg( data->getSamplingInfo(), fourierSize, 0, 2 );
   WaveAnalysis::StftData::Ptr stftData = transformAlg.execute( *data );

   Visualisation::RebinnedSRGraph graph( *stftData );
   graph.create( "testSpectralReassignment/SrGraph" );

   for ( size_t iHop = 0; iHop < stftData->getNumSpectra(); ++iHop )
   {
      const WaveAnalysis::SrSpectrum& srSpec = stftData->getSrSpectrum( iHop );

      FeatureAlgorithm::SrSpecPeakAlgorithm peakAlg;
      std::vector< Feature::SrSpecPeak > peaks = peakAlg.execute( srSpec );

      // WaveAnalysis::StftData::WindowLocation windowLoc = stftData->getWindowLocation( iHop );
      for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
      {
         const Feature::SrSpecPeak& peak = peaks[ iPeak ];

         RealVector timeVec = realVector( iHop, iHop + 1 );
         RealVector freqVec = realVector( peak.getFrequency(), peak.getFrequency() );

         gPlotFactory().createGraph( timeVec, freqVec, Qt::white );
      }
   }
   return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devPeakSustainAlgorithm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::devPeakSustainAlgorithm()
{
   Logger msg( "devPeakSustainAlgorithm" );
   msg << Msg::Info << "Running devPeakSustainAlgorithm..." << Msg::EndReq;

   std::unique_ptr< std::vector< Music::Note > > trueMelody( new std::vector< Music::Note >() );
   RawPcmData::Ptr data = TestDataSupply::generateRandomMelody( trueMelody.get() );
   // RawPcmData::Ptr data = TestDataSupply::getCurrentTestSample();

   size_t fourierSize = 1024;

   WaveAnalysis::SpectralReassignmentTransform transformAlg( data->getSamplingInfo(), fourierSize, 0, 2 );
   WaveAnalysis::StftData::Ptr stftData = transformAlg.execute( *data );

   std::vector< std::vector< Feature::IBasicSpectrumPeak* > > allPeaks( stftData->getNumSpectra() );
   for ( size_t iHop = 0; iHop < stftData->getNumSpectra(); ++iHop )
   {
      const WaveAnalysis::SrSpectrum& srSpec = stftData->getSrSpectrum( iHop );

      FeatureAlgorithm::SrSpecPeakAlgorithm peakAlg;
      std::vector< Feature::SrSpecPeak > peaks = peakAlg.execute( srSpec );
      allPeaks[ iHop ].reserve( peaks.size() );
      msg << Msg::Verbose << "Peaks.size() = " << peaks.size() << Msg::EndReq;
      for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
      {
         allPeaks[ iHop ].push_back( new Feature::SrSpecPeak( peaks[ iPeak ] ) );
      }
   }

   Visualisation::RebinnedSRGraph graph( *stftData );
   graph.create( "testSpectralReassignment/SrGraph" );

   FeatureAlgorithm::PeakSustainAlgorithm peakSustainAlg;
   const std::vector< Feature::SustainedPeak* >& sustainedPeaks = peakSustainAlg.execute( allPeaks );

   msg << Msg::Info << "Number of sustained peaks found = " << sustainedPeaks.size() << Msg::EndReq;

   for ( size_t i = 0; i < sustainedPeaks.size(); ++i )
   {
      msg << Msg::Info << "Sustained peak " << i << ":" << Msg::EndReq;
      msg << Msg::Info << "t0 = " << sustainedPeaks[ i ]->getStartTimeSamples() << ", t1 = " << sustainedPeaks[ i ]->getEndTimeSamples() << Msg::EndReq;
      msg << Msg::Info << "Mean freq = " << sustainedPeaks[ i ]->getFrequency() << Msg::EndReq;
      msg << Msg::Info << "Consists of " << sustainedPeaks[ i ]->getAllPeaks().size() << " peaks." << Msg::EndReq;
      for ( size_t iSubPeak = 0; iSubPeak < sustainedPeaks[ i ]->getAllPeaks().size(); ++iSubPeak )
      {
         const Feature::IBasicSpectrumPeak& subPeak = *sustainedPeaks[ i ]->getAllPeaks()[ iSubPeak ];
         msg << Msg::Info << " > A = " << subPeak.getHeight() << ", f = " << subPeak.getFrequency() << Msg::EndReq;
      }

      size_t startPeak = sustainedPeaks[ i ]->getStartTimeSamples();
      size_t endPeak = sustainedPeaks[ i ]->getEndTimeSamples();
      double freq = sustainedPeaks[ i ]->getFrequency();

      gPlotFactory().createGraph( realVector( startPeak, endPeak ), realVector( freq, freq ), Qt::white );
   }


   for ( size_t iSustainedPeak = 0; iSustainedPeak < sustainedPeaks.size(); ++iSustainedPeak )
   {
      delete sustainedPeaks[ iSustainedPeak ];
   }

   for ( size_t i = 0; i < allPeaks.size(); ++i )
   {
      for ( size_t j = 0; j < allPeaks[ i ].size(); ++j )
      {
         delete allPeaks[ i ][ j ];
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devFundamentalFreqFinder
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::devFundamentalFreqFinder()
{
   Logger msg( "devFundamentalFreqFinder" );
   msg << Msg::Info << "Running devFundamentalFreqFinder..." << Msg::EndReq;

   RandomNumberGenerator rng( 1 );

   double pDetect = 0.8;
   double baseFreq = 100;
   double freqBlur = 5;

   gPlotFactory().createPlot( "Toy model of detected Fourier frequencies" );

   RealVector frequencies;

   for ( size_t i = 1; i < 40; ++i )
   {
      bool isDetect = rng.uniform() < pDetect;
      if ( !isDetect )
      {
         continue;
      }

      double freq = i * baseFreq + rng.uniform( -freqBlur, freqBlur );
      frequencies.push_back( freq );

      msg << Msg::Info << "Frequency: " << freq << Msg::EndReq;

      gPlotFactory().createGraph( realVector( freq, freq ), realVector( 0, 1 ) );
   }

   Math::ApproximateGcdAlgorithm gcdAlg( 50 );

   Math::ApproximateGcdAlgorithm::Result result = gcdAlg.execute( frequencies );

   msg << Msg::Info << "Basefrequency = " << result.gcd << Msg::EndReq;

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devImprovedPeakAlgorithm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::devImprovedPeakAlgorithm()
{
   Logger msg( "devImprovedPeakAlgorithm" );
   msg << Msg::Info << "Running devImprovedPeakAlgorithm..." << Msg::EndReq;

   SamplingInfo samplingInfo;
   Synthesizer::SineGenerator sineGen( samplingInfo );
   Math::Log10Function log10Fct;

   RealVector freqsToGenerate = Utils::createRangeReal( 1000, 1100, 100 );
   size_t fourierSize = 1024;

   WaveAnalysis::SpectralReassignmentTransform srAlg( samplingInfo, fourierSize, 0, 1 );
   gPlotFactory().createPlot( "plot" );

   const RealVector& relFreqSampling = Utils::createRangeReal( 0.75, 1.25, 100 );

   std::vector< RealVector > relFreqAmps;

   Plotting::Palette pal = Plotting::Palette::heatPalette();

   for ( size_t iFreq = 0; iFreq < freqsToGenerate.size(); ++iFreq )
   {
      double freq = freqsToGenerate[ iFreq ];
      sineGen.setFrequency( freq );
      RawPcmData::Ptr data = sineGen.generate( 1024 * 4 + 46 );

      const WaveAnalysis::StftData::Ptr stftData = srAlg.execute( *data );
      const WaveAnalysis::SrSpectrum& spec = stftData->getSrSpectrum( 0 );

      RealVector xData = ( spec.getFrequencies() /*- spec.getFrequencyCorrections()*/ ) / freq;
      RealVector yData = spec.getMagnitude();

      Math::LinearInterpolator interp( xData, yData );
      relFreqAmps.push_back( interp.evalMany( relFreqSampling ) );
      gPlotFactory().createGraph( relFreqSampling, log10Fct.evalMany( relFreqAmps.back() ), pal.getColour( 1.0 * iFreq / freqsToGenerate.size() ) );
   }



//   gPlotFactory().createPlot( "test" );
//   gPlotFactory().drawPcmData( *data );
//
//   size_t fourierSize = 1024;
//
//   WaveAnalysis::SpectralReassignmentTransform srAlg( samplingInfo, fourierSize, 0, 1 );
//   const WaveAnalysis::StftData::Ptr stftData = srAlg.execute( *data );
//
//   Math::Log10Function log10Fct;
//
//   for ( size_t iSpec = 0; iSpec < 1 /*stftData->getNumSpectra()*/; ++iSpec )
//   {
//      const WaveAnalysis::SrSpectrum& srSpec = stftData->getSrSpectrum( iSpec );
//      std::ostringstream msg;
//      msg << "Spectrum " << iSpec;
//
//      Math::KernelPdf kernPdf( Math::IPdf::CPtr( new Math::GaussPdf( 0, 10 ) ), srSpec.getFrequencies(), srSpec.getMagnitude() );
//      Math::IRealFunction::Ptr kernPdfFunc = kernPdf.getDensityAsFunc();
//
//      RealVector modKernWeights = srSpec.getTimeCorrections();
//      for ( size_t i = 0; i < modKernWeights.size(); ++i )
//      {
//         modKernWeights[ i ] = fabs( modKernWeights[ i ] );
//      }
//      modKernWeights = 1 - modKernWeights/fourierSize;
//
//      Math::KernelPdf modKernPdf( Math::IPdf::CPtr( new Math::GaussPdf( 0, 10 ) ), srSpec.getFrequencies(), pwm( modKernWeights, srSpec.getMagnitude() ) );
//      Math::IRealFunction::Ptr modKernPdfFunc = modKernPdf.getDensityAsFunc();
//
//      const RealVector& originalFreqs = srSpec.getFrequencies() - srSpec.getFrequencyCorrections();
//      const RealVector& freqs = Utils::createRangeReal( originalFreqs.front(), originalFreqs.back(), 4000 );
//
//      double integral = sumElements( srSpec.getMagnitude() );
//      double integralPdf = sumElements( kernPdfFunc->evalMany( originalFreqs ) );
//
//      gPlotFactory().createPlot( msg.str() );
//      gPlotFactory().createGraph( freqs, log10Fct.evalMany( kernPdfFunc->evalMany( freqs ) / integralPdf ) );
//      gPlotFactory().createGraph( freqs, log10Fct.evalMany( modKernPdfFunc->evalMany( freqs ) / integralPdf ), Qt::red );
//      gPlotFactory().createGraph( originalFreqs, log10Fct.evalMany( srSpec.getMagnitude() / integral ), Qt::blue );
//      gPlotFactory().createScatter( srSpec.getFrequencies(), log10Fct.evalMany( srSpec.getMagnitude() / integral ), Plotting::MarkerDrawAttr( Qt::red ) );
//      gPlotFactory().createScatter( srSpec.getFrequencies(), srSpec.getTimeCorrections() );
//
//      gPlotFactory().createPlot( msg.str() + " (reassigned)" );
//      gPlotFactory().createScatter( srSpec.getFrequencies(), log10Fct.evalMany( srSpec.getMagnitude() ) );
//      gPlotFactory().createGraph( srSpec.getFrequencies(), log10Fct.evalMany( srSpec.getMagnitude() ), Qt::red );
//      gPlotFactory().createGraph( originalFreqs, log10Fct.evalMany( srSpec.getMagnitude() ), Qt::blue );
//
//      gPlotFactory().createPlot( msg.str() + " (original)" );
//      // gPlotFactory().createScatter( srSpec.getFrequencies(), log10Fct.evalMany( srSpec.getMagnitude() ) );
//      gPlotFactory().createGraph( originalFreqs, log10Fct.evalMany( srSpec.getMagnitude() ), Qt::blue );
//   }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devTimeStretcher
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::devTimeStretcher()
{
   Logger msg( "devTimeStretcher" );
   msg << Msg::Info << "Running devTimeStretcher..." << Msg::EndReq;

   // std::unique_ptr< std::vector< Music::Note > > trueMelody( new std::vector< Music::Note >() );
   // RawPcmData::Ptr data = TestDataSupply::generateRandomMelody( trueMelody.get() );
   // RawPcmData::Ptr data = TestDataSupply::getCurrentTestSample();

   SamplingInfo samplingInfo;
   Synthesizer::SineGenerator sineGen( samplingInfo );
   sineGen.setFrequency( 440 );
   RawPcmData::Ptr data = sineGen.generate( 1024 * 128 + 46 );

   Music::TimeStretcher timeStretchAlg( 2 );
   RawPcmData stretchedMusic = timeStretchAlg.execute( *data );

   gPlotFactory().createPlot( "RegeneratedWaveFile" );
   gPlotFactory().drawPcmData( *data );
   gPlotFactory().drawPcmData( stretchedMusic, Qt::red );

   data->normaliseToPeak();
   stretchedMusic.normaliseToPeak();

   MultiChannelRawPcmData waveData( new RawPcmData( *data ) );
   MultiChannelRawPcmData waveDataStretched( new RawPcmData( stretchedMusic ) );

   // WaveFile::write( "OriginalBeforeStretch.wav", waveData );
   // WaveFile::write( "StretchedMusic.wav", waveDataStretched );
}
