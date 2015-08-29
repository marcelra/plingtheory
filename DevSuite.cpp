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

   // devImprovedPeakAlgorithm();
   devSidelobeRejection();

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
#include "WindowLocation.h"
#include "SortCache.h"
#include "NaivePeaks.h"
#include "Peak.h"

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



void DevSuite::devSidelobeRejection()
{
   Logger msg( "devSidelobeRejection" );
   msg << Msg::Info << "Running devSidelobeRejection..." << Msg::EndReq;

   size_t fourierSize = 4096;

   const SamplingInfo samplingInfo;

   Synthesizer::SineGenerator sinGen( samplingInfo );
   sinGen.setFrequency( 440 );

   RawPcmData::Ptr waveData = sinGen.generate( fourierSize );
   WaveAnalysis::SpectralReassignmentTransform transformAlg( samplingInfo, fourierSize, 3*fourierSize, 2 );

   const WaveAnalysis::StftData::Ptr stftData = transformAlg.execute( *waveData );
   const WaveAnalysis::SrSpectrum& spec = stftData->getSrSpectrum( 0 );

   Math::Log10Function log10;

   gPlotFactory().createPlot( "devSidelobeRejection/Spectrum" );
   gPlotFactory().createGraph( spec.getFrequencies() - spec.getFrequencyCorrections(), spec.getMagnitude() );

   const RealVector& magnitudeVec = spec.getMagnitude();
   FeatureAlgorithm::NaivePeaks peakAlg( FeatureAlgorithm::NaivePeaks::Max );
   std::vector< Feature::Peak* > peaks = peakAlg.execute( magnitudeVec );
   for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
   {
      const Feature::Peak& peak = *peaks[ iPeak ];
      msg << Msg::Info << "Peak " << iPeak << " with height " << peak.getPedestal() << Msg::EndReq;
      delete peaks[ iPeak ];
   }

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devImprovedPeakAlgorithm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::devImprovedPeakAlgorithm()
{
   Logger msg( "devImprovedPeakAlgorithm" );
   msg << Msg::Info << "Running devImprovedPeakAlgorithm..." << Msg::EndReq;

   // Take fourier bin size = 4096
   // Take sine with phase = 0
   // Make center frequency variable
   // Pick range around center frequency with delta_freq
   // Compute max rel amp as function of freq delta (envelope)
   // First sort by amp, remove all non-increasing frequency entries

   size_t fourierSize = 4096;
   size_t zeroPadSize = 3*fourierSize;

   const RealVector& centreFreqs = realVector( 400 );
   double deltaRange = 100;
   size_t numFreqs = 100;
   const double freqDelta = deltaRange / numFreqs;

   SamplingInfo samplingInfo;
   Synthesizer::SineGenerator sineGen( samplingInfo );

   RealVector relMaxY;
   RealVector diffX;

   for ( size_t iCentreFreq = 0; iCentreFreq < centreFreqs.size(); ++iCentreFreq )
   {
      for ( size_t iFreq = 0; iFreq < numFreqs; ++iFreq )
      {
         double frequency = centreFreqs[ iCentreFreq ] + freqDelta * iFreq;
         sineGen.setFrequency( frequency );
         RawPcmData::Ptr data = sineGen.generate( fourierSize );

         WaveAnalysis::SpectralReassignmentTransform transform( samplingInfo, fourierSize, zeroPadSize, 2 );
         WaveAnalysis::StftData::Ptr stftData = transform.execute( *data );
         const WaveAnalysis::SrSpectrum& spec = stftData->getSrSpectrum( 0 );

         const RealVector& x = spec.getFrequencies();
         RealVector&& y = spec.getMagnitude();

         y /= Utils::getMaxValue( y );
         RealVector deltaX = x - frequency;
         // msg << Msg::Info << deltaX << Msg::EndReq;

         relMaxY.insert( relMaxY.end(), y.begin(), y.end() );
         diffX.insert( diffX.end(), deltaX.begin(), deltaX.end() );
      }
   }

   SortCache relYSc( diffX );
   const RealVector& relMaxYSorted = relYSc.applyTo( relMaxY );
   const RealVector& relDiffXSorted = relYSc.applyTo( diffX );

   Math::Log10Function log10;

   gPlotFactory().createPlot( "envelope" );
   gPlotFactory().createGraph( relDiffXSorted, relMaxYSorted );

   // Create envelope by requiring linear interpolation is always above curve.
   size_t nEnvelopeSamples = 500;

   RealVector linearGrid = Utils::createRangeReal( -1000, 1000, nEnvelopeSamples );
   RealVector zeros = RealVector( nEnvelopeSamples, 0 );

   gPlotFactory().createScatter( linearGrid, zeros, Plotting::MarkerDrawAttr( Qt::red ) );


   gPlotFactory().createPlot( "envelopLog" );
   gPlotFactory().createGraph( relDiffXSorted, log10.evalMany( relMaxYSorted ) );



   // RealVector fCorr = spec.getFrequencyCorrections();
   // for ( size_t i = 0; i < spec.getFrequencies().size(); ++i )
   // {
   //    fCorr[ i ] /= spec.getFrequency( i );
   // }


   // Math::Log10Function log10Fct;

   // gPlotFactory().createPlot( "plot1" );
   // gPlotFactory().createGraph( x, log10Fct.evalMany( y ), Qt::blue );
   // gPlotFactory().createScatter( x, log10Fct.evalMany( y ) );
   // gPlotFactory().createGraph( x, log10Fct.evalMany( fCorr ), Qt::green );
   // gPlotFactory().createScatter( x, log10Fct.evalMany( fCorr ), Plotting::MarkerDrawAttr( Qt::red ) );

   // msg << Msg::Info << "Number of spectra in STFT: " << stftData->getNumSpectra() << Msg::EndReq;
   // for ( size_t iSpec = 0; iSpec < stftData->getNumSpectra(); ++iSpec )
   // {
   //    const WaveAnalysis::WindowLocation* windowLoc = stftData->getSpectrum( iSpec ).getWindowLocation();
   //    msg << Msg::Info << "WindowLocation: [" << windowLoc->getFirstSample() << ", " << windowLoc->getLastSample() << "]" << Msg::EndReq;
   // }



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
