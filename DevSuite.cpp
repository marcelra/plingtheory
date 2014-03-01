#include "DevSuite.h"
#include "Logger.h"
#include "TestDataSupply.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// execute
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::execute()
{
   Logger msg( "DevSuite" );
   msg << Msg::Info << "Running development code..." << Msg::EndReq;
   // devRebinSRSpec();
   // devPeakFinder2();
   devFourierTemplates();
   // devSamples();

   /// PARKED
   // devSidelobeSubtraction();
   // devEntropyPeaks();

   /// PARKED AND INTERESTING
   // devFourierPeakFinder1();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Include section
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <set>
#include "NoteList.h"
#include "AccumArrayPeakAlgorithm.h"
#include "GroundtoneHypothesisBuilder.h"
#include "RebinnedSRGraph.h"
#include "RegularAccumArray.h"
#include "SortCache.h"
#include "TwoTuple.h"
#include "SampledMovingAverage.h"
#include "TLine.h"
#include "WaveFile.h"
#include "MultiChannelRawPcmData.h"
#include "StftGraph.h"
#include "RawPcmData.h"
#include "FourierSpectrum.h"
#include "FourierTransform.h"
#include "SineGenerator.h"
#include "SquareGenerator.h"
#include "RootUtilities.h"
#include "Utils.h"
#include "SpectralReassignmentTransform.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Parked code fragments
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "DevSuite.parked.cpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devRebinSRSpec
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::devRebinSRSpec()
{
   Logger msg( "devRebinSRSpec" );
   msg << Msg::Info << "In devRebinSRSpec..." << Msg::EndReq;

   RawPcmData::Ptr data = TestDataSupply::getCurrentTestSample();

   size_t monitorIndex = 5;

   size_t fourierSize = 4096;
   WaveAnalysis::SpectralReassignmentTransform transform( data->getSamplingInfo(), fourierSize, fourierSize*7, 4 );
   WaveAnalysis::StftData::Ptr stftData = transform.execute( *data );

   WaveAnalysis::SrSpectrum& spec = static_cast< WaveAnalysis::SrSpectrum& >( stftData->getSpectrum( monitorIndex ) );

   const Math::RegularAccumArray& accArr = spec.rebinToFourierLattice();

   SortCache sortX( spec.getFrequencies() );

   TH1F* hAccArr = accArr.createHistogram();

   new TCanvas();
   hAccArr->Draw();
   // TGraph* grUnbinned = RootUtilities::createGraph( sortX.applyTo( spec.getFrequencies() ), sortX.applyTo( spec.getMagnitude() ) );
   // grUnbinned->SetLineColor( kRed + 2 );
   // grUnbinned->SetMarkerColor( kRed + 2 );
   // grUnbinned->Draw( "PSAME" );

   WaveAnalysis::StftAlgorithm normalTransAlg( data->getSamplingInfo(), fourierSize, WaveAnalysis::HanningWindowFuncDef(), 0, 2 );
   WaveAnalysis::StftData::Ptr normalTrans = normalTransAlg.execute( *data );
   // const WaveAnalysis::FourierSpectrum& normalSpec = normalTrans->getSpectrum( 5 );
   // TGraph* grNormal = RootUtilities::createGraph( normalSpec.getFrequencies(), normalSpec.getMagnitude() );
   // grNormal->Draw( "LSAME" );
   // grNormal->SetLineColor( kBlue );

   // Visualisation::RebinnedSRGraph rebinnedGraph( *stftData, stftData->getNumSpectra(), 0 );
   // rebinnedGraph.create();

   const WaveAnalysis::SrSpectrum& testSpec = dynamic_cast< WaveAnalysis::SrSpectrum& >( stftData->getSpectrum( monitorIndex ) );

   FeatureAlgorithm::GroundtoneHypothesisBuilder gtSeed( testSpec );
   const RealVector& vec = gtSeed.execute( 20 );
   msg << Msg::Info << vec << Msg::EndReq;

}

void DevSuite::devPeakFinder2()
{
   Logger msg( "devPeakFinder2" );
   msg << Msg::Info << "Running devPeakFinder2..." << Msg::EndReq;

   // const Math::RegularAccumArray& data = TestDataSupply::drawNoiseAndPeaks();
   size_t specIndex = 5;
   WaveAnalysis::StftData::Ptr stftData = TestDataSupply::getSrFtData();
   const WaveAnalysis::SrSpectrum& spec = dynamic_cast< WaveAnalysis::SrSpectrum& >( stftData->getSpectrum( specIndex ) );
   const Math::RegularAccumArray& data = spec.rebinToFourierLattice();

   // TH1F* hist = data.createHistogram();
   // hist->Draw();

   FeatureAlgorithm::AccumArrayPeakAlgorithm peakAlg;
   peakAlg.setDoMonitor( true );
   const std::vector< Feature::Peak>& peaks = peakAlg.execute( data );

   RealVector prominence( peaks.size() );
   RealVector frequencyBin( peaks.size() );
   RealVector width( peaks.size() );
   for ( size_t i = 0; i < peaks.size(); ++i )
   {
      prominence[ i ] = peaks[ i ].getProminence();
      frequencyBin[ i ] = peaks[ i ].getPosition();
      width[ i ] = peaks[ i ].getWidth();
      msg << Msg::Verbose << "i = " << i << Msg::EndReq;
      msg << Msg::Verbose << "Prominence = " << peaks[ i ].getProminence() << Msg::EndReq;
   }

   new TCanvas();
   TGraph* grPromVsFreq = RootUtilities::createGraph( prominence, frequencyBin );
   grPromVsFreq->Draw( "AP" );
   new TCanvas();
   TGraph* grWidthVsFreq = RootUtilities::createGraph( width, frequencyBin );
   grWidthVsFreq->Draw( "AP" );

   return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devFourierTemplates
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::devFourierTemplates()
{
   Logger msg( "devFourierTemplates" );
   msg << Msg::Info << "In devFourierTempaltes..." << Msg::EndReq;

   Music::NoteList noteList = Music::createHugeDiatonicScale();
   size_t numHarmonics = 10;
   double amp = 1;
   SamplingInfo samplingInfo( 44100 );
   size_t fourierSize = 2048;
   size_t zeroPadding = 7*fourierSize;
   size_t numSamplesGenerate = 4096;
   double hopRate = 1;

   WaveAnalysis::SpectralReassignmentTransform transform( samplingInfo, fourierSize, zeroPadding, hopRate );

   std::set< double > frequencies;

   for ( size_t iNote = 0; iNote < noteList.size(); ++iNote )
   {
      for ( size_t iHarmonic = 0; iHarmonic < numHarmonics; ++iHarmonic )
      {
         frequencies.insert( noteList[ iNote ].getFrequency() * ( iHarmonic + 1 ) );
      }
   }

   double freqMinDraw = 0;
   double freqMaxDraw = samplingInfo.getNyquistFrequency();

   size_t counter = 0;
   size_t numGraphs = 0;
   size_t graphSkip = 20;
   size_t numGraphsDrawn = 0;
   bool interactiveGraphs = true;
   if ( !interactiveGraphs )
   {
      graphSkip = 1;
   }
   for ( std::set< double >::iterator it = frequencies.begin(); it != frequencies.end(); ++it, ++counter )
   {

      double frequency = *it;
      if ( frequency > samplingInfo.getNyquistFrequency() )
      {
         break;
      }
      msg << Msg::Info << "Creating fourier templates for frequency " << frequency << "." << Msg::EndReq;

      msg << Msg::Debug << "Generating waveform." << Msg::EndReq;

      RawPcmData pcmData( samplingInfo, numSamplesGenerate );

      double phase = 0;
      double phaseStep = samplingInfo.getPhaseStepPerSample( frequency );
      for ( size_t iSample = 0; iSample < numSamplesGenerate; ++iSample )
      {
         pcmData[ iSample ] = amp * sin( phase );
         phase += phaseStep;
      }

      msg << Msg::Debug << "Fourier transforming waveform." << Msg::EndReq;
      WaveAnalysis::StftData::Ptr stftData = transform.execute( pcmData );
      const WaveAnalysis::SrSpectrum& srSpectrum = dynamic_cast< const WaveAnalysis::SrSpectrum& >( stftData->getSpectrum( 0 ) );
      const Math::RegularAccumArray& accArr = srSpectrum.rebinToFourierLattice();
      TH1F* hist = accArr.createHistogram();

      msg << Msg::Debug << "Generating delayed waveform." << Msg::EndReq;
      RawPcmData delayedData( samplingInfo, numSamplesGenerate, 0 );
      phase = 0;
      for ( size_t iSample = 0; /*fourierSize / 4*/ iSample < fourierSize / 2; ++iSample )
      {
         delayedData[ iSample ] = amp * sin( phase );
         phase += phaseStep;
      }
      WaveAnalysis::StftData::Ptr stftDataDelayed = transform.execute( delayedData );
      const WaveAnalysis::SrSpectrum& srSpectrumDelayed = dynamic_cast< const WaveAnalysis::SrSpectrum& >( stftDataDelayed->getSpectrum( 0 ) );
      const Math::RegularAccumArray& accArrDelayed = srSpectrumDelayed.rebinToFourierLattice();
      TH1F* histDelayed = accArrDelayed.createHistogram();

      if ( frequency > freqMinDraw && frequency < freqMaxDraw )
      {
         ++numGraphs;
         if ( numGraphs % graphSkip == 0 )
         {
            ++numGraphsDrawn;
            std::stringstream name;
            name << "templateCanvas" << counter;
            std::stringstream title;
            title << "Frequency = " << *it;
            TCanvas* c = new TCanvas( name.str().c_str(), title.str().c_str() );
            // c->SetLogy( true );

            // if ( frequency < 1500 )
            // {
            //    c->SetLogx( true );
            // }

            msg << Msg::Info << counter << " histograms drawn." << Msg::EndReq;

            hist->Draw();
            hist->SetLineColor( kBlack );
            TGraph* grHatT = RootUtilities::createGraph( srSpectrum.getFrequencies(), srSpectrum.getTimeCorrections() );
            grHatT->Draw( "PSAME" );
            grHatT->SetMarkerColor( kBlack );


            histDelayed->Draw( "SAME" );
            histDelayed->SetLineColor( kBlue );

            TGraph* grHatTDelayed = RootUtilities::createGraph( srSpectrumDelayed.getFrequencies(), srSpectrumDelayed.getTimeCorrections() );
            grHatTDelayed->SetMarkerColor( kBlue );
            grHatTDelayed->Draw( "PSAME" );

            TGraph* grHatOmegaDelayed = RootUtilities::createGraph( srSpectrumDelayed.getFrequencies(), 10 * srSpectrumDelayed.getFrequencyCorrections() );
            grHatOmegaDelayed->SetMarkerColor( kRed );
            grHatOmegaDelayed->Draw( "PSAME" );

            if ( !interactiveGraphs )
            {
               name << ".gif";
               c->SaveAs( name.str().c_str() );
               delete c;
            }
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devSamples
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::devSamples()
{
   size_t numSamples = 10 * 44100;

   SamplingInfo samplingInfo( 44100 );
   RawPcmData pcmData( samplingInfo, numSamples, 0 );

   double frequency = 110;
   double phase = 0;
   double amp = 0.5;
   double phaseStep = samplingInfo.getPhaseStepPerSample( frequency );

   size_t i = 0;
   while ( i < numSamples )
   {
      size_t tStart = samplingInfo.convertSecsToSamples( 0.1 ) + i;
      size_t tEnd = samplingInfo.convertSecsToSamples( 0.50 ) + i;
      size_t tRestart = samplingInfo.convertSecsToSamples( 1.0 ) + i;
      double phaseAdd = M_PI / ( tEnd - tStart );


      for ( ; i < tStart && i < numSamples ; ++i )
      {
         pcmData[ i ] = amp * sin( phase );
         phase += phaseStep;
      }

      phaseStep += phaseAdd;
      for ( ; i < tEnd && i < numSamples; ++i )
      {
         pcmData[ i ] = amp * sin( phase );
         phase += phaseStep;
      }

      phaseStep -= phaseAdd;
      for ( ; i < tRestart && i < numSamples; ++i )
      {
         pcmData[ i ] = amp * sin( phase );
         phase += phaseStep;
      }

      ++i;
   }

   TGraph* grData = RootUtilities::createGraph( pcmData );
   grData->Draw( "AL" );

   WaveAnalysis::SpectralReassignmentTransform transform( samplingInfo, 4096, 3*4096, 4 );
   WaveAnalysis::StftData::Ptr stft = transform.execute( pcmData );

   Visualisation::RebinnedSRGraph graph( *stft );
   graph.create();

   MultiChannelRawPcmData mc( new RawPcmData( pcmData ) );
   WaveFile::write( "sinefrag2.wav", mc );
}
