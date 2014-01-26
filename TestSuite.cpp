#include "TestSuite.h"

#include "Exceptions.h"
#include "GlobalParameters.h"

#include "EffectTrianglizer.h"
#include "FourierFilter.h"
#include "MonophonicSimpleRandomMusicGenerator.h"
#include "RollingBufferSawtoothTransform.h"
#include "RootUtilities.h"
#include "SineGenerator.h"
#include "SquareGenerator.h"
#include "WaveFile.h"

#include "Note.h"
#include "NoteList.h"
#include "FourierNoteFilter.h"
#include "FftwAlgorithm.h"
#include "FourierSpectrum.h"
#include "ShortTimeFftw.h"
#include "ObjectPool.h"
#include "Peak.h"
#include "Tone.h"
#include "TestMath.h"
#include "NaivePeaks.h"
#include "FocalTones.h"
#include "AlgorithmBase.h"
#include "StftGraph.h"
#include "DevSpectralReassignment.h"
#include "WaveletTransform.h"
#include "WaveletContainer.h"
#include "DynamicFourier.h"
#include "ResonanceMatrixVisualisation.h"
#include "AdvancedFourierTransform.h"
#include "StftAlgorithm.h"
#include "AdsrEnvelope.h"
#include "NoiseGenerator.h"
#include "TriangleGenerator.h"
#include "SawtoothGenerator.h"

#include "TLine.h"
#include "TH2F.h"

#include <iostream>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////
/// runCurrentDevelopmentTest
////////////////////////////////////////////////////////////////////////////////
void TestSuite::runCurrentDevelopmentTest()
{
   // testSoundData();
   // testWaveFile();
   // testNote();
   // testSineGenerator();
   // testRandomMusic();
   // runAllTests();
   // testFourierFilter();
   // testFourierNoteFilter();
   // testFourierNoteFilterExternalFile();
   // testShortTimeFftw();
   // testObjectPool();
   // testFeatureData();
   // testPeakDetection();
   // testAlgorithmFramework();
   // testIntegration();

   // testShortTimeFftw();
   // testDynamicFourier();
   // testAdvancedFourier();
   // testStftAlgorithm();

   // runTestMath();
   // Dev::test();
   // testEnvelope();
   // testNoiseGenerator();
   // testTriangleGenerator();
   testSawtoothGenerator();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// runTestMath
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::runTestMath()
{
   Logger msg( "runTestMath" );
   msg << Msg::Info << "Running Math tests..." << Msg::EndReq;
   TestMath::testMath();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// generateRandomMusic
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RawPcmData::Ptr TestSuite::generateRandomMusic()
{
   Music::NoteList noteArray4 = Music::createMinorScale( Music::Note(Music::Note::C, 4, Music::Duration(0.25) ) );
   Music::NoteList noteArray5 = Music::createMinorScale( Music::Note(Music::Note::C, 5, Music::Duration(0.25) ) );

   Music::NoteList noteArray;
   noteArray.insert( noteArray.end(), noteArray4.begin(), noteArray4.end() );
   noteArray.insert( noteArray.end(), noteArray5.begin(), noteArray5.end() );

   SamplingInfo samplingInfo( 44100 );
   Synthesizer::SquareGenerator synth( samplingInfo );
   synth.setAmplitude(0.5);

   Music::MonophonicSimpleRandomMusicGenerator randomMusic( noteArray, 1 );
   randomMusic.useSynthesizer( &synth);

   Music::NoteList generatedNotes;
   RawPcmData::Ptr musicData = randomMusic.generateRandomMusic( 20, &generatedNotes );
   return musicData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testObjectPool
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testObjectPool()
{
   Logger msg( "testObjectPool" );
   msg << Msg::Info << "Running testObjectPool..." << Msg::EndReq;
   {
      ObjectPool& op = ObjectPool::getInstance();
      TestObject* to = new TestObject();
      to->i = 42;
      op.store( "TestObject", to );
      op.dump( std::cout );
   }
   {
      ObjectPool& op = ObjectPool::getInstance();
      const TestObject* to = 0;
      op.retrieve( "TestObject", to );
      if ( to->i != 42 )
      {
         throw ExceptionTestFailed( "testObjectPool", "Could not reproduce value that was inserted." );
      }
      msg << Msg::Info << "TestObject.i = " << to->i << " (should be 42). " << Msg::EndReq;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testAlgorithmFramework
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testAlgorithmFramework()
{
   Logger msg ( "testAlgorithmFramework" );
   msg << Msg::Info << "Running testAlgorithmFramework..." << Msg::EndReq;

   AlgorithmBase alg( "AlgBase", "Test" );
   alg.execute();

}

////////////////////////////////////////////////////////////////////////////////
/// runAllTests
////////////////////////////////////////////////////////////////////////////////
void TestSuite::runAllTests()
{
   Logger msg("runAllTests");
   msg << Msg::Info << "Running all tests..." << Msg::EndReq;
   testEffectTrianglizer();
   testPartialIntegralTransform();
   testSoundData();
   testWaveFile();
   testNote();
   testSineGenerator();
}

////////////////////////////////////////////////////////////////////////////////
/// testEffectTrianglizer
////////////////////////////////////////////////////////////////////////////////
void TestSuite::testEffectTrianglizer()
{
   Logger msg("testEffectTrianglizer");
   msg << Msg::Info << "Running testEffectTrianglizer..." << Msg::EndReq;
   MultiChannelRawPcmData* inputData = WaveFile::read( "/Users/marcelra/Dev/Suite_soundArchive/tmp.wav" );
   MultiChannelRawPcmData* outputData = new MultiChannelRawPcmData();
   for ( size_t iChannel = 0; iChannel < inputData->getNumChannels(); ++iChannel )
   {
      RawPcmData::Ptr data = EffectTrianglizer::apply( inputData->getChannel( iChannel ) );
      outputData->addChannel( data.release() );
   }

   WaveFile::write( "test.wav", *outputData );

   delete inputData;
   delete outputData;
}

////////////////////////////////////////////////////////////////////////////////
/// testPartialIntegralTransform
////////////////////////////////////////////////////////////////////////////////
void TestSuite::testPartialIntegralTransform()
{
   Logger msg("testPartialIntegralTransform");
   msg << Msg::Info << "Running testPartialIntegralTransform..." << Msg::EndReq;
   RawPcmData data( SamplingInfo(), 10000, 0);

   double period = 25 * pow(2, 1/12.);
   double phase = 0;
   double phaseStep = 2*acos(-1)/25;
   double amplitude = 2000;

   for ( size_t i = 0; i < data.size(); ++i )
   {
      data[i] = amplitude * sin( phase );
      phase += phaseStep;
   }

   size_t nPeriods = 50;

   RollingBufferSawtoothTransform( data, period, nPeriods );
}

////////////////////////////////////////////////////////////////////////////////
/// testSoundData
////////////////////////////////////////////////////////////////////////////////
void TestSuite::testSoundData()
{
   Logger msg( "testSoundData" );
   msg << Msg::Info << "Running testSoundData..." << Msg::EndReq;
   MultiChannelRawPcmData soundData;

   RawPcmData* leftData = new RawPcmData( SamplingInfo(), 10 );
   RawPcmData* rightData = new RawPcmData( SamplingInfo(), 10 );

   double lambda = 9;

   for ( size_t i = 0; i < leftData->size(); ++i )
   {
      (*leftData)[i] = sin( i/lambda * 2 * M_PI );
      (*rightData)[i] = cos( i/lambda * 2 * M_PI );
   }

   soundData.addChannel( leftData );
   soundData.addChannel( rightData );

   msg << Msg::Info << "Number of channels created = " << soundData.getNumChannels() << Msg::EndReq;
   msg << Msg::Info << "Created " << soundData.getNumSamples() << " samples" << Msg::EndReq;
   for ( size_t i = 0; i < soundData.getNumSamples(); ++i )
   {
      for ( size_t iChannel = 0; iChannel < soundData.getNumChannels(); ++iChannel )
      {
         RawPcmData& storedChannel = soundData.getChannel( iChannel );
         msg << Msg::Info << "Channel " << iChannel << ": sample value = " << storedChannel[i] << Msg::EndReq;
      }
   }
   msg << Msg::Info << "Test complete" << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////
/// testWaveFile
////////////////////////////////////////////////////////////////////////////////
void TestSuite::testWaveFile()
{
   Logger msg( "testWaveFile" );
   msg << Msg::Info << "Running testWaveFile" << Msg::EndReq;

   WaveFile waveFile;
   try
   {
      msg << Msg::Info << "Test exception handling. Note that this will give a WARNING and an ERROR." << Msg::EndReq;
      MultiChannelRawPcmData* dat = waveFile.read( "thisWaveFileDoesNotExist" );
      /// Will not get here!
      assert( dat );
   }
   catch ( ExceptionFileNotFound exc )
   {
      msg << Msg::Error << exc << Msg::EndReq;
   }

   try
   {
      WaveFile waveFile;
      /// Test reading
      MultiChannelRawPcmData* data = waveFile.read( GlobalParameters::getTrunkPath() + "inputData/scarredSolo.wav" );

      /// Test writing
      waveFile.write( GlobalParameters::getTrunkPath() + "testResults/testSoundData.wav", *data );

      /// Make graph of read data
      std::vector< double > pcmData;
      size_t offset = 44100;
      for ( size_t i = offset; i < offset + 10000; ++i )
      {
         pcmData.push_back( data->getChannel(0)[i] );
      }
      TGraph* graph = RootUtilities::createGraph( pcmData );
      new TCanvas( "canWaveFile", "canWaveFile" );
      graph->Draw( "AL" );

      /// Clean up
      delete data;
   }
   catch ( ExceptionFileNotFound exc )
   {
      msg << Msg::Error << exc.getMessage() << Msg::EndReq;
      assert( false );
   }
}

void TestSuite::testNote()
{
   Logger msg( "testNote" );
   msg << Msg::Info << "Running testNote..." << Msg::EndReq;

   Music::Note a(Music::Note::A, 4, Music::Duration(1) );
   Music::Note c(Music::Note::C, 4, Music::Duration(1) );

   msg << Msg::Info << "Frequency of A4 = " << a.getFrequency() << Msg::EndReq;
   msg << Msg::Info << "Frequency of C4 = " << c.getFrequency() << Msg::EndReq;
   c.modifyPitch( +9 );
   msg << Msg::Info << "Frequency of C4 + 9 semitones = " << c.getFrequency() << Msg::EndReq;
   c.modifyPitch( -9 );
   msg << Msg::Info << "Frequency of C4 + 9 - 9 semitones = " << c.getFrequency() << Msg::EndReq;
   c.modifyPitch( -3 );
   msg << Msg::Info << "Frequency of C4 - 3 semitones = " << c.getFrequency() << Msg::EndReq;

   c.modifyPitch( 24 );
   c.modifyPitch( -24 );
   msg << Msg::Info << "Frequency of A4 +24 - 24 semitones = " << c.getFrequency() << Msg::EndReq;
   assert( fabs( c.getFrequency() - a.getFrequency() ) < 1e-12 );

   Music::Note b4( Music::Note::B, 4, Music::Duration(1) );
   Music::Note b4Recon( b4.getFrequency() );
   msg << Msg::Info << "Frequency of B4 = " << b4.getFrequency() << " and of B4 recon = " << b4Recon.getFrequency() << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testSineGenerator
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testSineGenerator()
{
   Logger msg( "testSineGenerator" );
   msg << Msg::Info << "Running testSineGenerator..." << Msg::EndReq;

   /// Define output format
   SamplingInfo samplingInfo;
   RawPcmData* channel = new RawPcmData( samplingInfo );

   /// Setup synthesizer and notes to play
   Synthesizer::SineGenerator sine( samplingInfo );
   sine.setAmplitude( 0.5 );

   Music::Note c( Music::Note::C, 4 );
   Music::Note es( Music::Note::Es, 4 );

   size_t oneSec = 44100;
   RawPcmData::Ptr noteData;

   /// Generate 'c'-note
   sine.setFrequency( c.getFrequency() );
   noteData = sine.generate( 2 * oneSec );
   channel->pasteAtEnd( *noteData );

   /// Generate 'es'-note
   sine.setFrequency( es.getFrequency() );
   noteData = sine.generate( 2 * oneSec );
   channel->pasteAtEnd( *noteData );

   /// Write to wave file
   MultiChannelRawPcmData data( channel );
   WaveFile::write( "testSineGen.wav", data );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testRandomMusic
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testRandomMusic()
{
   Logger msg( "testRandomMusic" );
   msg << Msg::Info << "Running testRandomMusic..." << Msg::EndReq;

   SamplingInfo samplingInfo( 44100 );
   size_t numNotesToGenerate = 400;

   Music::Note rootAccomp( Music::Note::C, 4, Music::Duration(1) );
   Music::Note rootBass( Music::Note::C, 3, Music::Duration(4) );
   Music::Note rootSolo = Music::Note( Music::Note::C, 5, Music::Duration( 0.25 ) );

   Music::NoteList noteArray = Music::createMinorScale( rootAccomp );
   Music::NoteList soloNoteArray = Music::createMinorScale( rootSolo );
   Music::NoteList bassNoteArray = Music::createMinorScale( rootBass );

   /// Generate accomp
   Music::MonophonicSimpleRandomMusicGenerator msrmg( noteArray, 1 );
   RawPcmData::Ptr generatedDataLeft = msrmg.generateRandomMusic( numNotesToGenerate );
   RawPcmData::Ptr generatedDataRight = msrmg.generateRandomMusic( numNotesToGenerate );

   /// Generate solo
   Music::MonophonicSimpleRandomMusicGenerator soloGen( soloNoteArray, 4 );
   Synthesizer::SquareGenerator squareSynth( samplingInfo );
   soloGen.useSynthesizer( &squareSynth );
   squareSynth.setAmplitude( 0.05 );
   RawPcmData::Ptr generatedSoloData = soloGen.generateRandomMusic( numNotesToGenerate*4 );

   /// Generate bass
   Music::MonophonicSimpleRandomMusicGenerator bassGen( bassNoteArray, 3 );
   bassGen.useSynthesizer( &squareSynth );
   squareSynth.setAmplitude( 0.03 );
   RawPcmData::Ptr generatedBassData = bassGen.generateRandomMusic( numNotesToGenerate/4 );

   /// Mix
   generatedSoloData->resize( generatedDataLeft->size() );
   generatedDataLeft->mixAdd( *generatedSoloData );
   generatedDataRight->mixAdd( *generatedSoloData );

   generatedDataLeft->mixAdd( *generatedBassData );
   generatedDataRight->mixAdd( *generatedBassData );

   MultiChannelRawPcmData waveData( generatedDataLeft.release(), generatedDataRight.release() );
   WaveFile::write( GlobalParameters::getTrunkPath() + "testResults/randomMonoMusic.wav", waveData );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testFourierFilter
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testFourierFilter()
{
   Logger msg( "testFourierFilter" );
   msg << Msg::Info << "Running testFourierFilter..." << Msg::EndReq;

   Music::Note noteA( Music::Note::A, 3, Music::Duration(1) );
   Music::Note noteB( Music::Note::B, 3, Music::Duration(1) );

   SamplingInfo samplingInfo( 44100 );
   size_t numSamples = samplingInfo.convertSecsToSamples( noteA.getDuration().getSeconds( 120 ) );

   Synthesizer::SineGenerator sineGen( samplingInfo );
   sineGen.setFrequency( noteA.getFrequency() );
   sineGen.setAmplitude( 0.4 );
   RawPcmData::Ptr firstNote = sineGen.generate( numSamples );
   sineGen.setFrequency( noteB.getFrequency() );
   sineGen.setAmplitude( 0 );
   RawPcmData::Ptr secondNote = sineGen.generate( numSamples );

   firstNote->pasteAtEnd( *secondNote );

   sineGen.setAmplitude( 0.4 );
   secondNote = sineGen.generate( numSamples );

   firstNote->pasteAtEnd( *secondNote );

   RawPcmData* data = firstNote.get();

   TGraph* graph = RootUtilities::createGraph( *data );
   new TCanvas( "waveCanvas", "waveCanvas" );
   graph->Draw( "AL" );

   WaveAnalysis::FourierFilter fourierFilterA( noteA.getFrequency(), 5, samplingInfo );
   RawPcmData::Ptr fourierDataA = fourierFilterA.apply( *data );


   Music::Note noteTest( Music::Note::B, 3, Music::Duration(1) );
   WaveAnalysis::FourierFilter fourierFilterTest( noteTest.getFrequency(), 5, samplingInfo );
   RawPcmData::Ptr fourierDataTest = fourierFilterTest.apply( *data );

   new TCanvas( "fourierCanvas", "fourierCanvas" );
   TGraph* graphFilterCorrect = RootUtilities::createGraph( *fourierDataA );
   graphFilterCorrect->Draw( "AL" );

   TGraph* graphFilterTest = RootUtilities::createGraph( *fourierDataTest );
   graphFilterTest->Draw( "LSAME" );
   graphFilterTest->SetLineColor( kRed );

   double aNoteAmp[4] = {0.05, 0.05, 0, 0};
   double aNoteX[4] = {0, 1.0*numSamples, numSamples+1.0, 3.0*numSamples};
   double bNoteAmp[4] = {0, 0, 0.05, 0.05};
   double bNoteX[4] = {0, 2.0*numSamples, 2*numSamples + 1.0, 3.0*numSamples};

   TGraph* gTrueA = new TGraph( 4, aNoteX, aNoteAmp );
   TGraph* gTrueB = new TGraph( 4, bNoteX, bNoteAmp );
   gTrueA->SetLineColor( kBlue );
   gTrueB->SetLineColor( kGreen );
   gTrueA->SetLineWidth( 2 );
   gTrueB->SetLineWidth( 2 );


   gTrueA->Draw( "LSAME" );
   gTrueB->Draw( "LSAME" );

   TLegend* legend = RootUtilities::createDefaultLegend();
   legend->AddEntry( gTrueA, "Note A", "l" );
   legend->AddEntry( gTrueB, "Note B", "l" );
   legend->AddEntry( graphFilterCorrect, "Filter A", "l" );
   legend->AddEntry( graphFilterTest, "Filter B", "l" );
   legend->Draw();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testFourierNoteFilter
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testFourierNoteFilter()
{
   Logger msg("testFourierNoteFilter");
   msg << Msg::Info << "Running testFourierNoteFilter..." << Msg::EndReq;

   RawPcmData::Ptr musicData = generateRandomMusic();

   WaveAnalysis::FourierNoteFilter noteFilter( *musicData );

   noteFilter.visualiseResults();

   // /// Reverse
   // RawPcmData* reverseData = new RawPcmData( musicData->getSamplingInfo(), musicData->size(), 0 );
   // noteFilter.reverseGenerate( *reverseData );
//
   // MultiChannelRawPcmData reverseOutput( reverseData );
   // WaveFile::write( "reverseData.wav", reverseOutput );
//
   // MultiChannelRawPcmData soundData( musicData.release() );
   // WaveFile::write( "test.wav", soundData );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testFourierNoteFilterExternalFile
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testFourierNoteFilterExternalFile()
{
   Logger msg( "testFourierNoteFilterExternalFile" );
   msg << Msg::Info << "Running testFourierFilterExternalFile..." << Msg::EndReq;

   MultiChannelRawPcmData* data = WaveFile::read( "/Users/marcelra/Dev/Suite_soundArchive/atc_intro.wav" );
   // MultiChannelRawPcmData* data = WaveFile::read( "/Users/marcelra/Dev/Suite_soundArchive/outcry.wav" );
   // MultiChannelRawPcmData* data = WaveFile::read( "/Users/marcelra/Dev/Suite_soundArchive/de_spaghetti_ontknoping.wav" );
   RawPcmData musicData = RawPcmData( data->getChannel( 0 ) );
   delete data;

   WaveAnalysis::FourierNoteFilter noteFilter( musicData );

   // TCanvas* can = noteFilter.visualiseResults();
   // can->Update();

   RawPcmData* reverseData = new RawPcmData( musicData.getSamplingInfo(), musicData.size()*2, 0 );
   noteFilter.executeAndReverse( *reverseData );

   MultiChannelRawPcmData reverseOutput( reverseData );
   WaveFile::write( "reverseData2.wav", reverseOutput );

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testFftw
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testFftw()
{
   Logger msg( "testFftw" );
   msg << Msg::Info << "Running testFftw..." << Msg::EndReq;

   /// Get sample wave-file
   MultiChannelRawPcmData* data = WaveFile::read( "/Users/marcelra/Dev/Suite_soundArchive/de_spaghetti_ontknoping.wav" );
   const RawPcmData* musicData = &data->getChannel( 0 );

   size_t nSamples = 512;
   WaveAnalysis::FftwAlgorithm fftw( nSamples );
   WaveAnalysis::FourierSpectrum::Ptr fsp = fftw.transform( *musicData, 0 );
   RawPcmData::Ptr original = fftw.transform( *fsp );

   /// Note that original has only 512 samples
   double tolerance = 1e-12;
   for ( size_t iSample = 0; iSample < original->size(); ++iSample )
   {
      if ( fabs( (*original)[iSample] - (*musicData)[iSample] ) > tolerance )
      {
         throw ExceptionTestFailed( "testFftw", "Reverse Fourier transform did not reproduce original!" );
      }
   }
   msg << Msg::Info << "Test passed!" << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testShortTimeFftw
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testShortTimeFftw()
{
   Logger msg( "testShortTimeFftw" );
   msg << Msg::Info << "Running testShortTimeFftw..." << Msg::EndReq;

   MultiChannelRawPcmData* data = WaveFile::read( "/Users/marcelra/Dev/Suite_soundArchive/atc_intro.wav" );
   const RawPcmData* musicData = &data->getChannel( 0 );
   // const RawPcmData::Ptr musicData = generateRandomMusic();

   msg << Msg::Info << "Applying fourier transforms..." << Msg::EndReq;
   size_t nSamples = 4096;
   WaveAnalysis::ShortTimeFftw stft( nSamples, *musicData, WaveAnalysis::RectangularWindowFuncDef() );
   msg << Msg::Info << "Fourier transforms done." << Msg::EndReq;

   // Visualisation::StftGraph stftGraph( stft );
   // stftGraph.create();

   // MultiChannelRawPcmData data( new RawPcmData( *musicData ) );
   // WaveFile::write( "musicData.wav", data );
   // delete data;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testFeatureData
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testFeatureData()
{
   Logger msg( "testFeatureData" );
   msg << Msg::Info << "Running testFeatureData..." << Msg::EndReq;

   Feature::Peak peak( 10, 0.5, 2, 0 );

   msg << Msg::Info << "Peak: pos = " << peak.getPosition()
                    << ", height = " << peak.getProminence()
                    << ", width = " << peak.getWidth() << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testPeakDetection
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testPeakDetection()
{
   Logger msg( "testPeakDetection" );
   msg << Msg::Info << "Running testPeakDetection..." << Msg::EndReq;

   size_t numPoints = 200;
   std::vector< double > array( numPoints, 0 );
   size_t numPeaks = 10;
   std::vector< double > means;
   for ( size_t iPeak = 0; iPeak < numPeaks; ++iPeak )
   {
      means.push_back( ( iPeak + 0.5 ) * ( 1.0 * numPoints ) / numPeaks );
   }

   double sigma = 5;
   for ( size_t iPoint = 0; iPoint < array.size(); ++iPoint )
   {
      double val = 0;
      for ( size_t iPeak = 0; iPeak < means.size(); ++iPeak )
      {
         double x = iPoint;
         double arg = ( x - means[iPeak] ) / 2 / sigma;
         val += 1/sqrt(2*M_PI)/sigma * exp( -2*arg*arg );
      }
      array[ iPoint ] = val;
   }

   TGraph* graph = RootUtilities::createGraph( array );
   graph->Draw( "AL" );

   FeatureAlgorithm::NaivePeaks peakAlg( array, FeatureAlgorithm::NaivePeaks::Max );

   msg << Msg::Info << "The number of peaks is: " << peakAlg.getNumPeaks() << Msg::EndReq;

   for ( size_t iPeak = 0; iPeak < peakAlg.getNumPeaks(); ++iPeak )
   {
      const Feature::Peak& peak = peakAlg.getPeak( iPeak );
      msg << Msg::Info << "Peak[" << iPeak << "] : pos = " << peak.getPosition()
                       << ", height = " << peak.getProminence()
                       << ", width = " << peak.getWidth() << Msg::EndReq;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testIntegration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testIntegration()
{
   Logger msg( "testShortTimeFftw" );
   msg << Msg::Info << "Running testShortTimeFftw..." << Msg::EndReq;

   MultiChannelRawPcmData* waveFile = WaveFile::read( "/Users/marcelra/Dev/Suite_soundArchive/monophDistGuitar2.wav" );
   RawPcmData& musicDataRef = waveFile->getChannel( 0 );
   RawPcmData* musicData = &musicDataRef;
   //const RawPcmData::Ptr musicData = generateRandomMusic();

   size_t nSamples = 2048;
   WaveAnalysis::ShortTimeFftw stft( nSamples, *musicData, WaveAnalysis::HannPoissonWindowFuncDef() );

   TH2F* result = new TH2F( "result", "result", stft.getNumSpectra(), -0.5, stft.getNumSpectra() - 0.5, stft.getSpectrumDimension(), -0.5, stft.getSpectrumDimension() );
   std::vector< TLine* > detectedTones;
   std::vector< Music::Note > detectedNotes;

   SamplingInfo samplingInfo( 44100 );
   RawPcmData* regeneratedData = new RawPcmData( samplingInfo );
   Synthesizer::SineGenerator synth( samplingInfo );
   synth.setAmplitude(0.5);

   for ( size_t iSpec = 0; iSpec < stft.getNumSpectra(); ++iSpec )
   {
      bool doMonitor = iSpec == 5;

      msg << Msg::Always << "Processing fourier spectrum " << iSpec << Msg::EndReq;
      WaveAnalysis::FourierSpectrum spec = stft.getSpectrum( iSpec );
      std::vector<double> specMag = spec.getMagnitude();
      // for ( size_t iLpFilter = 0; iLpFilter < 0; ++iLpFilter )
      // {
      //    specMag[iLpFilter] = 0;
      // }

      if ( doMonitor )
      {
         new TCanvas();
         TGraph* g = RootUtilities::createGraph( spec.getFrequencies(), spec.getMagnitude() );
         g->Draw( "AL" );
      }

      FeatureAlgorithm::NaivePeaks peakAlg( specMag, FeatureAlgorithm::NaivePeaks::Max, 20 );
      peakAlg.execute();

      if ( doMonitor )
      {
         for ( size_t iPeak = 0; iPeak < peakAlg.getNumPeaks(); ++iPeak )
         {
            const Feature::Peak& p = peakAlg.getPeak( iPeak );
            double x = spec.getFrequency( p.getPositionIndex() );
            TLine* line = new TLine( x, 0, x, 10000 );
            line->SetLineWidth( 2 );
            line->Draw();
         }
      }

      msg << Msg::Info << "The number of peaks is: " << peakAlg.getNumPeaks() << Msg::EndReq;

      FeatureAlgorithm::FocalTones naiveTones( peakAlg.getAllPeaks() );
      naiveTones.setDoMonitor( doMonitor );
      naiveTones.execute();

      for ( size_t iTone = 0; iTone < naiveTones.getNumTones(); ++iTone )
      {
         const Feature::Tone& tone = naiveTones.getTone( iTone );
         msg << Msg::Info << "Tone with base frequency: " << tone.getBaseFreq() << Msg::EndReq;
         for ( size_t iHarmonic = 0; iHarmonic < tone.getHarmonics().size(); ++iHarmonic )
         {
            msg << Msg::Info << " - harmonic with relative amp: " << tone.getHarmonics()[iHarmonic] << Msg::EndReq;
         }
         double freq = spec.getFrequency( tone.getBaseFreq() );
         Music::Note note( freq, Music::Duration( 0.125 ) );
         msg << Msg::Always << note.toString() << Msg::EndReq;
         detectedNotes.push_back( note );
         // synth.setFrequency( detectedNotes[iNote].getFrequency() );
         synth.setFrequency( note.getFrequency() );
         synth.setAmplitude( 0.1 );
         if ( iTone == 0 )
         {
            TLine* line = new TLine( iSpec - 0.5, tone.getBaseFreq(), iSpec + 0.5, tone.getBaseFreq() );
            line->SetLineColor( kBlack );
            line->SetLineWidth( 2 );
            detectedTones.push_back( line );
            regeneratedData->pasteAtEnd( *synth.generate( nSamples ) );
         }
         else
         {
            // regeneratedData->mixAdd( *synth.generate( nSamples, regeneratedData->getSamplingInfo() ), regeneratedData->size() - nSamples );
         }
      }

      for ( size_t i = 0; i < specMag.size(); ++i )
      {
         result->SetBinContent( iSpec + 1, i + 1, specMag[i] );
      }
   }

   result->Draw( "COLZ" );
   for ( size_t i = 0; i < detectedTones.size(); ++i )
   {
      detectedTones[i]->Draw();
   }

   MultiChannelRawPcmData data( new RawPcmData( *musicData ) );
   WaveFile::write( "musicData.wav", data );

   // for ( size_t iNote = 0; iNote < detectedNotes.size(); ++iNote )
   // {
   //    synth.setFrequency( detectedNotes[iNote].getFrequency() );
   //    regeneratedData->pasteAtEnd( *synth.generate( nSamples, regeneratedData->getSamplingInfo() ) );
   // }

   MultiChannelRawPcmData dataRegen( regeneratedData );
   WaveFile::write( "dataRegen.wav", dataRegen );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testDynamicFourier
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testDynamicFourier()
{
   Logger msg( "testDynamicFourier" );
   msg << Msg::Info << "Running testDynamicFourier..." << Msg::EndReq;

   MultiChannelRawPcmData* waveFile = WaveFile::read( "/Users/marcelra/Dev/Suite_soundArchive/monophDistGuitar.wav" );
   // MultiChannelRawPcmData* waveFile = WaveFile::read( "/Users/marcelra/Dev/Suite_soundArchive/atc_intro.wav" );
   RawPcmData& musicDataRef = waveFile->getChannel( 0 );
   RawPcmData* data = &musicDataRef;
   // RawPcmData::Ptr data = generateRandomMusic();

   const Music::NoteList notes = Music::createHugeDiatonicScale();
   // Music::NoteList notes;
   // notes.push_back( Music::Note( Music::Note::G, 4 ) );
   // notes.push_back( Music::Note( Music::Note::Gis, 4 ) );
   // notes.push_back( Music::Note( Music::Note::A, 4 ) );
   size_t numOvertones = 0;
   std::vector< double > freqList;
   for ( size_t iNote = 0; iNote < notes.size(); ++iNote )
   {
      const Music::Note& note = notes[iNote];
      freqList.push_back( note.getFrequency() );
      for ( size_t iOvertone = 0; iOvertone < numOvertones; ++iOvertone )
      {
         freqList.push_back( note.getFrequency()*( (iOvertone + 2) ) );
      }
   }

   WaveAnalysis::DynamicFourier dynFour( freqList, 31 );
   const WaveAnalysis::ResonanceMatrix& resMatrix = dynFour.execute( *data );
   Visualisation::ResonanceMatrixVisualisation resVis( resMatrix );
   TCanvas* can = resVis.draw( 1000 );
   can->Update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testAdvancedFourier
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testAdvancedFourier()
{
   Logger msg( "testAdvancedFourier" );
   msg << Msg::Info << "Running testAdvancedFourier..." << Msg::EndReq;

   SamplingInfo samplingInfo( 44100 );
   Synthesizer::SineGenerator sinGen( samplingInfo );

   sinGen.setFrequency( 440 );
   sinGen.setAmplitude( 0.5 );

   RawPcmData::Ptr data = sinGen.generate( 44100 );

   size_t windowSize = 8192;
   WaveAnalysis::AdvancedFourierTransform ftAlg( samplingInfo, windowSize, WaveAnalysis::HanningWindowFuncDef(), 3 * windowSize );

   WaveAnalysis::AdvancedFourierSpectrum::Ptr spec = ftAlg.transform( &(*data)[0] );
   RealVectorPtr dataRev = ftAlg.transform( *spec );
   RealVectorPtr dataVec = data->copyToVectorData();
   dataVec->resize( 8192 );

   RealVector dataSub = *dataRev - *dataRev;

   TGraph* gr = RootUtilities::createGraph( spec->getFrequencies(), spec->getMagnitude() );
   new TCanvas( "AdvancedFourierTransform", "AdvancedFourierTransform" );
   gr->Draw( "AL" );

   TGraph* grRevCheck = RootUtilities::createGraph( dataSub );
   new TCanvas( "ReverseCheck", "ReverseCheck" );
   grRevCheck->Draw( "AL" );


}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testStftAlgorithm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testStftAlgorithm()
{
   Logger msg( "testStftAlgorithm" );
   msg << Msg::Info << "Running testStftAlgorithm..." << Msg::EndReq;

   SamplingInfo samplingInfo( 44100 );
   Synthesizer::SineGenerator sinGen( samplingInfo );

   sinGen.setFrequency( 440 );
   sinGen.setAmplitude( 0.5 );
   // RawPcmData::Ptr data = sinGen.generate( 44100, samplingInfo );

   // RawPcmData::Ptr data = generateRandomMusic(); // sinGen.generate( 44100, samplingInfo );
   MultiChannelRawPcmData* mcData = WaveFile::read( "/Users/marcelra/Dev/Suite_soundArchive/atc_intro.wav" );
   // MultiChannelRawPcmData* mcData = WaveFile::read( "/home/marcelra/mac/Dev/Suite_soundArchive/atc_intro.wav" );
   // MultiChannelRawPcmData* mcData = WaveFile::read( "/home/marcelra/mac/Dev/Suite_soundArchive/monophDifficult.wav" );
   // MultiChannelRawPcmData* mcData = WaveFile::read( "/home/marcelra/mac/Dev/Suite_soundArchive/outcry.wav" );
   RawPcmData* data = &mcData->getChannel( 0 );

   size_t windowSize = 8096;
//  WaveAnalysis::StftConfiguration stftConf( samplingInfo, windowSize, WaveAnalysis::HanningWindowFuncDef(), 3 * windowSize, 8 );

   WaveAnalysis::StftAlgorithm stftAlg( samplingInfo, windowSize, WaveAnalysis::HanningWindowFuncDef(), 3 * windowSize, 2 );
   stftAlg.execute( *data );

   for ( size_t i = 0; i < stftAlg.getNumSpectra(); ++i )
   {
      const WaveAnalysis::StftAlgorithm::WindowLocation& winLoc = stftAlg.getWindowLocation( i );
      const WaveAnalysis::StftAlgorithm::WindowLocation& winLocNoOv = stftAlg.getWindowLocationNoOverlap( i );
      msg << Msg::Info << "-------------------------------------------------------" << Msg::EndReq;
      msg << Msg::Info << "Spectrum i = " << i << ": Windowlocation [" << winLoc.getFirstSample() << ", " << winLoc.getLastSample() << "]" << Msg::EndReq;
      msg << Msg::Info << "Spectrum i = " << i << ": No overlap     [" << winLocNoOv.getFirstSample() << ", " << winLocNoOv.getLastSample() << "]" << Msg::EndReq;
   }

   const WaveAnalysis::AdvancedFourierSpectrum& spec = stftAlg.getSpectrum( 0 );
   new TCanvas();
   TGraph* gr = RootUtilities::createGraph( spec.getFrequencies(), spec.getMagnitude() );
   gr->Draw( "AL" );

   Visualisation::StftGraph stftGraph( stftAlg );
   stftGraph.create();

   RawPcmData::Ptr reverse = stftAlg.reverseExecute();
   msg << Msg::Info << data->size() << ", " << reverse->size() << Msg::EndReq;

   new TCanvas();
   RealVectorPtr dataVec = data->copyToVectorData();
   RealVectorPtr reverseVec = reverse->copyToVectorData();
   reverseVec->resize( dataVec->size() );
   TGraph* grSub = RootUtilities::createGraph(  *dataVec - *reverseVec );
   grSub->Draw( "AL" );

   delete mcData;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testEnvelope
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testEnvelope()
{
   Logger msg( "testEnvelope" );
   msg << Msg::Info << "Running testEnvelope..." << Msg::EndReq;

   Synthesizer::AdsrEnvelope envelope( 100, 50, 400, 0.8, 50 );
   RealVector v( 1000 );
   for ( size_t i = 0; i < v.size(); ++i )
   {
      v[i] = envelope.getEnvelope( i );
   }

   new TCanvas();
   TGraph* gr = RootUtilities::createGraph( v );
   gr->Draw( "AL" );

   SamplingInfo samplingInfo( 44100 );
   Synthesizer::SineGenerator square( samplingInfo );
   square.setFrequency( 880 * 4 );
   square.setEnvelope( new Synthesizer::AdsrEnvelope( envelope ) );
   RawPcmData::Ptr data = square.generate( 1000 );

   RootUtilities::createGraph( *data )->Draw( "AL" );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testNoiseGenerator
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testNoiseGenerator()
{
   SamplingInfo samplingInfo( 44100 );
   Synthesizer::NoiseGenerator noiseGen( samplingInfo );
   noiseGen.setAmplitude( 1 );
   RawPcmData::Ptr data = noiseGen.generate( 44100 );

   Synthesizer::SquareGenerator toneGen( samplingInfo );
   toneGen.setAmplitude( 1 );
   toneGen.setFrequency( 440 );
   toneGen.setEnvelope( new Synthesizer::AdsrEnvelope( 10000, 5000, 10000, 0.5, 5000 ) );
   data->mixAdd( *toneGen.generate( 44100 ) );

   TGraph* gr = RootUtilities::createGraph( *data );
   new TCanvas();
   gr->Draw( "AL" );

   size_t windowSize = 4096;
   WaveAnalysis::StftAlgorithm stftAlg( samplingInfo, windowSize, WaveAnalysis::HanningWindowFuncDef(), windowSize, 2 );
   stftAlg.execute( *data );

   Visualisation::StftGraph stftGraph( stftAlg );
   stftGraph.create();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testTriangleGenerator
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testTriangleGenerator()
{
   Logger msg( "testTriangleGenerator" );
   msg << Msg::Info << "Running testTriangleGenerator..." << Msg::EndReq;

   SamplingInfo samplingInfo( 44100 );
   Synthesizer::TriangleGenerator toneGen( samplingInfo );
   toneGen.setAmplitude( 1 );
   toneGen.setFrequency( 440 );
   toneGen.setEnvelope( new Synthesizer::AdsrEnvelope( 10000, 5000, 10000, 0.5, 5000 ) );
   toneGen.setPhase( 0 );

   RawPcmData dataCombined( samplingInfo );
   for ( size_t i = 0; i < 100; ++i )
   {
      dataCombined.pasteAtEnd( *toneGen.generate( 44100 / 100 ) );
   }

   toneGen.setPhase( 0 );
   toneGen.resetEnvelopePhase();
   RawPcmData::Ptr data = toneGen.generate( 44100 );

   TGraph* grRaw = RootUtilities::createGraph( *data );
   new TCanvas();
   grRaw->Draw( "AL" );

   RealVectorPtr correctData = data->copyToVectorData();
   RealVectorPtr testData = dataCombined.copyToVectorData();
   TGraph* grPhaseCheck = RootUtilities::createGraph( *testData );
   new TCanvas();
   grPhaseCheck->Draw( "AL" );

   MultiChannelRawPcmData triangleMcRpcm( new RawPcmData( dataCombined ) );
   WaveFile::write( "triangle.wav", triangleMcRpcm );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testSawtoothGenerator
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testSawtoothGenerator()
{
   Logger msg( "testSawtoothGenerator" );
   msg << Msg::Info << "Running testSawtoothGenerator..." << Msg::EndReq;

   SamplingInfo samplingInfo( 44100 );
   Synthesizer::SawtoothGenerator toneGen( samplingInfo );
   toneGen.setAmplitude( 1 );
   toneGen.setFrequency( 1071 );
   // toneGen.setEnvelope( new Synthesizer::AdsrEnvelope( 10000, 5000, 10000, 0.5, 5000 ) );
   toneGen.setPhase( 0 );

   RawPcmData dataCombined( samplingInfo );
   for ( size_t i = 0; i < 100; ++i )
   {
      dataCombined.pasteAtEnd( *toneGen.generate( 44100 / 100 ) );
   }

   toneGen.setPhase( 0 );
   toneGen.resetEnvelopePhase();
   RawPcmData::Ptr data = toneGen.generate( 44100 );

   TGraph* grRaw = RootUtilities::createGraph( *data );
   new TCanvas();
   grRaw->Draw( "AL" );

   RealVectorPtr correctData = data->copyToVectorData();
   RealVectorPtr testData = dataCombined.copyToVectorData();
   TGraph* grPhaseCheck = RootUtilities::createGraph( *testData );
   new TCanvas();
   grPhaseCheck->Draw( "AL" );

   MultiChannelRawPcmData sawtoothMcRpcm( new RawPcmData( dataCombined ) );
   WaveFile::write( "sawtooth.wav", sawtoothMcRpcm );

   WaveAnalysis::AdvancedFourierTransform ft( samplingInfo, 4096 * 10, WaveAnalysis::RectangularWindowFuncDef(), 0 );
   double* arr = &((*data)[0]);
   WaveAnalysis::AdvancedFourierSpectrum::Ptr spec = ft.transform( arr );
   TGraph* grSpec = RootUtilities::createGraph( spec->getFrequencies(), spec->getMagnitude() );
   new TCanvas();
   grSpec->Draw( "AL" );

   // WaveAnalysis::StftAlgorithm stftAlg( samplingInfo, 4096, WaveAnalysis::RectangularWindowFuncDef(), 4096, 2 );
   // stftAlg.execute( dataCombined );
   // Visualisation::StftGraph stftGraph( stftAlg );
   // stftGraph.create();
}
