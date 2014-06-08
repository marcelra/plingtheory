#include "TestSuite.h"

#include "Exceptions.h"
#include "GlobalParameters.h"
#include "IPlotFactory.h"
#include "RootUtilities.h"

/// TODO: handle these items.
bool checkTodos = false;

////////////////////////////////////////////////////////////////////////////////
/// runCurrentDevelopmentTest
////////////////////////////////////////////////////////////////////////////////
void TestSuite::execute()
{
   // singleTest();
   runTestMath();

   /// Utilities
   testFindMinima();
   testRandomNumberGenerator();

   /// Base classes
   testSoundData();
   testWaveFile();
   testNote();

   /// Infrastucture
   testObjectPool();
   testThreading();

   /// Generators
   testEnvelope();
   testSineGenerator();
   testNoiseGenerator();
   testTriangleGenerator();
   testSawtoothGenerator();
   testRandomMusic();

   /// WaveAnalysis
   testAdvancedFourier();
   testStftAlgorithm();
   testSpectralReassignment();

   /// Feature algorithms
   testPeakDetection();

   /// Everything together
   testIntegration();

   /// Plotting
   testHist2D();
   testHistogram();

   return;
   /// Expensive tests
   // testDynamicFourier();
}

void TestSuite::singleTest()
{
   testFindMinima();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Include section
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "EffectTrianglizer.h"
#include "MonophonicSimpleRandomMusicGenerator.h"
#include "RollingBufferSawtoothTransform.h"
#include "SineGenerator.h"
#include "SquareGenerator.h"
#include "WaveFile.h"
#include "MultiChannelRawPcmData.h"

#include "Note.h"
#include "NoteList.h"
#include "FftwAlgorithm.h"
#include "GaussPdf.h"
#include "ObjectPool.h"
#include "Peak.h"
#include "Tone.h"
#include "TestMath.h"
#include "NaivePeaks.h"
#include "FocalTones.h"
#include "AlgorithmBase.h"
#include "StftGraph.h"
#include "DynamicFourier.h"
#include "Regular2DHistogram.h"
#include "ResonanceMatrixVisualisation.h"
#include "FourierTransform.h"
#include "StftAlgorithm.h"
#include "AdsrEnvelope.h"
#include "NoiseGenerator.h"
#include "TriangleGenerator.h"
#include "RandomNumberGenerator.h"
#include "RealMemFunction.h"
#include "RebinnedSRGraph.h"
#include "SawtoothGenerator.h"
#include "StftData.h"
#include "SpectralReassignmentTransform.h"
#include "IThread.h"

#include "TLine.h"
#include "TH2F.h"

#include <iostream>
#include <math.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// runTestMath
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::runTestMath()
{
   Logger msg( "runTestMath" );
   msg << Msg::Info << "Running Math tests..." << Msg::EndReq;
   TestMath::execute();
   msg << Msg::Info << "Math tests done." << Msg::EndReq;
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
/// testThreading
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testThreading()
{
   Logger msg( "testThread" );
   msg << Msg::Info << "Running testThread..." << Msg::EndReq;

   IThread thread( "testThread" );
   thread.start();

   msg << Msg::Info << "This message comes from outside the thread." << Msg::EndReq;
   msg << Msg::Info << "About to join the thread..." << Msg::EndReq;

   thread.join();
   msg << Msg::Info << "The thread has been joined." << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testEffectTrianglizer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testEffectTrianglizer()
{
   Logger msg("testEffectTrianglizer");
   msg << Msg::Info << "Running testEffectTrianglizer..." << Msg::EndReq;
   MultiChannelRawPcmData* inputData = WaveFile::read( GlobalParameters::getTestDataDir() + "tmp.wav" );
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
      msg << Msg::Error << "Test exception handling. Note that this will give a WARNING and an ERROR (this is ok)" << Msg::EndReq;
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
      MultiChannelRawPcmData* data = waveFile.read( GlobalParameters::getTestDataDir() + "anotherDay.wav" );

      /// Test writing
      waveFile.write( GlobalParameters::getRunDir() + "testSoundData.wav", *data );

      /// Make graph of read data
      std::vector< double > pcmData;
      size_t offset = 44100;
      for ( size_t i = offset; i < offset + 10000; ++i )
      {
         pcmData.push_back( data->getChannel(0)[i] );
      }

      gPlotFactory().createPlot( "testWaveFile/WaveFileView" );
      gPlotFactory().createGraph( pcmData );

      /// Clean up
      delete data;
   }
   catch ( ExceptionFileNotFound exc )
   {
      msg << Msg::Error << exc.getMessage() << Msg::EndReq;
      throw ExceptionTestFailed( "testWaveFile", "No wave file found" );
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
   WaveFile::write( GlobalParameters::getRunDir() + "testResults/randomMonoMusic.wav", waveData );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testFftw
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testFftw()
{
   Logger msg( "testFftw" );
   msg << Msg::Info << "Running testFftw..." << Msg::EndReq;

   /// Get sample wave-file
   MultiChannelRawPcmData* data = WaveFile::read( GlobalParameters::getTestDataDir() + "de_spaghetti_ontknoping.wav" );
   const RawPcmData* musicData = &data->getChannel( 0 );

   size_t nSamples = 512;
   WaveAnalysis::FourierTransform ft( data->getSamplingInfo(), nSamples );
   WaveAnalysis::FourierSpectrum::Ptr fsp = ft.transform( &(*musicData)[0] );
   RealVectorPtr original = ft.transform( *fsp );

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

   MultiChannelRawPcmData* waveFile = WaveFile::read( GlobalParameters::getTestDataDir() + "monophDistGuitar2.wav" );
   RawPcmData& musicDataRef = waveFile->getChannel( 0 );
   RawPcmData* musicData = &musicDataRef;
   //const RawPcmData::Ptr musicData = generateRandomMusic();

   size_t nSamples = 2048;
   WaveAnalysis::StftAlgorithm stft( musicData->getSamplingInfo(), nSamples, WaveAnalysis::HannPoissonWindowFuncDef(), nSamples * 3, 2 );
   WaveAnalysis::StftData::Ptr stftData = stft.execute( *musicData );

   TH2F* result = new TH2F( "result", "result", stftData->getNumSpectra(), -0.5, stftData->getNumSpectra() - 0.5, stft.getSpectrumDimension(), -0.5, stft.getSpectrumDimension() );
   std::vector< TLine* > detectedTones;
   std::vector< Music::Note > detectedNotes;

   SamplingInfo samplingInfo( 44100 );
   RawPcmData* regeneratedData = new RawPcmData( samplingInfo );
   Synthesizer::SineGenerator synth( samplingInfo );
   synth.setAmplitude(0.5);

   for ( size_t iSpec = 0; iSpec < stftData->getNumSpectra(); ++iSpec )
   {
      bool doMonitor = iSpec == 5;

      // msg << Msg::Always << "Processing fourier spectrum " << iSpec << Msg::EndReq;
      WaveAnalysis::FourierSpectrum spec = stftData->getSpectrum( iSpec );
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
         Music::Note note( freq, Music::Duration( 0.125 / 2 ) );
         // msg << Msg::Always << note.toString() << Msg::EndReq;
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

   MultiChannelRawPcmData* waveFile = WaveFile::read( GlobalParameters::getTestDataDir() + "monophDistGuitar.wav" );
   // MultiChannelRawPcmData* waveFile = WaveFile::read( GlobalParameters::getTestDataDir() + "atc_intro.wav" );
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
   WaveAnalysis::FourierTransform ftAlg( samplingInfo, windowSize, WaveAnalysis::HanningWindowFuncDef(), 3 * windowSize );

   WaveAnalysis::FourierSpectrum::Ptr spec = ftAlg.transform( &(*data)[0] );
   RealVectorPtr dataRev = ftAlg.transform( *spec );
   RealVectorPtr dataVec = data->copyToVectorData();
   dataVec->resize( 8192 );

   const RealVector& dataSub = *dataRev - *dataRev;

   gPlotFactory().createPlot( "testAdvancedFourier/FT" );
   gPlotFactory().createGraph( spec->getFrequencies(), spec->getMagnitude() );

   for ( size_t i = 0; i < dataSub.size(); ++i )
   {
      if ( fabs( dataSub[ i ] ) > 1e-14 )
      {
         throw ExceptionTestFailed( "testAdvancedFourier", "Reverse Fourier transform did not yield original result." );
      }
   }
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
   MultiChannelRawPcmData* mcData = WaveFile::read( GlobalParameters::getTestDataDir() + "atc_intro.wav" );
   // MultiChannelRawPcmData* mcData = WaveFile::read( GlobalParameters::getTestDataDir() + "atc_intro.wav" );
   // MultiChannelRawPcmData* mcData = WaveFile::read( GlobalParameters::getTestDataDir() + "monophDifficult.wav" );
   // MultiChannelRawPcmData* mcData = WaveFile::read( GlobalParameters::getTestDataDir() + "outcry.wav" );
   RawPcmData* data = &mcData->getChannel( 0 );

   size_t windowSize = 8096;
//  WaveAnalysis::StftConfiguration stftConf( samplingInfo, windowSize, WaveAnalysis::HanningWindowFuncDef(), 3 * windowSize, 8 );

   WaveAnalysis::StftAlgorithm stftAlg( samplingInfo, windowSize, WaveAnalysis::HanningWindowFuncDef(), 3 * windowSize, 2 );
   WaveAnalysis::StftData::Ptr stftData = stftAlg.execute( *data );

   for ( size_t i = 0; i < stftData->getNumSpectra(); ++i )
   {
      const WaveAnalysis::StftData::WindowLocation& winLoc = stftData->getWindowLocation( i );
      const WaveAnalysis::StftData::WindowLocation& winLocNoOv = stftData->getWindowLocationNoOverlap( i );
      msg << Msg::Info << "-------------------------------------------------------" << Msg::EndReq;
      msg << Msg::Info << "Spectrum i = " << i << ": Windowlocation [" << winLoc.getFirstSample() << ", " << winLoc.getLastSample() << "]" << Msg::EndReq;
      msg << Msg::Info << "Spectrum i = " << i << ": No overlap     [" << winLocNoOv.getFirstSample() << ", " << winLocNoOv.getLastSample() << "]" << Msg::EndReq;
   }

   const WaveAnalysis::FourierSpectrum& spec = stftData->getSpectrum( 0 );
   gPlotFactory().createPlot( "testStftAlgorithm/First FT" );
   gPlotFactory().createGraph( spec.getFrequencies(), spec.getMagnitude() );

   if ( checkTodos )
   {
      assert( !"Uncomment STFT graph code" );
   }
   // Visualisation::StftGraph stftGraph( *stftData );
   // stftGraph.create();

   RawPcmData::Ptr reverse = stftAlg.reverseExecute( *stftData );

   msg << Msg::Info << "The reverse STFT produced " << reverse->size() << " samples." << Msg::EndReq;
   msg << Msg::Info << "The original contains " << data->size() << " samples." << Msg::EndReq;

   const RealVector& vecReverse = Utils::createSelection( *reverse->copyToVectorData(), Utils::createRange( 0, data->size() ) );

   const RealVector& dataSub = *data->copyToVectorData() - vecReverse;

   for ( size_t i = 0; i < dataSub.size(); ++i )
   {
      if ( fabs( dataSub[ i ] ) > 1e-14 )
      {
         throw ExceptionTestFailed( "testStftAlgorithm", "Reverse Fourier transform did not yield original result." );
      }
   }

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

   SamplingInfo samplingInfo( 44100 );
   Synthesizer::SquareGenerator square( samplingInfo );
   square.setFrequency( 880 * 4 );
   square.setEnvelope( new Synthesizer::AdsrEnvelope( envelope ) );
   RawPcmData::Ptr data = square.generate( 1000 );

   gPlotFactory().createPlot( "testEnvelope/Envelope" );
   gPlotFactory().createGraph( *data->copyToVectorData() );
   gPlotFactory().createGraph( v );
   gPlotFactory().createGraph( -1 * v );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testNoiseGenerator
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testNoiseGenerator()
{
   Logger msg( "testNoiseGenerator" );
   msg << Msg::Info << "Running testNoiseGenerator..." << Msg::EndReq;

   SamplingInfo samplingInfo( 44100 );
   Synthesizer::NoiseGenerator noiseGen( samplingInfo, 1 );
   noiseGen.setAmplitude( 1 );
   RawPcmData::Ptr data = noiseGen.generate( 44100 );

   Synthesizer::SquareGenerator toneGen( samplingInfo );
   toneGen.setAmplitude( 1 );
   toneGen.setFrequency( 440 );
   toneGen.setEnvelope( new Synthesizer::AdsrEnvelope( 10000, 5000, 10000, 0.5, 5000 ) );
   data->mixAdd( *toneGen.generate( 44100 ) );

   gPlotFactory().createPlot( "testNoiseGenerator/PCM" );
   gPlotFactory().drawPcmData( *data );

   size_t windowSize = 4096;
   WaveAnalysis::StftAlgorithm stftAlg( samplingInfo, windowSize, WaveAnalysis::HanningWindowFuncDef(), windowSize, 2 );
   WaveAnalysis::StftData::Ptr stftData = stftAlg.execute( *data );

   if ( checkTodos )
   {
      assert( !"Enable code below" );
   }
   // Visualisation::StftGraph stftGraph( *stftData );
   // stftGraph.create();
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

   RealVectorPtr correctData = data->copyToVectorData();
   RealVectorPtr testData = dataCombined.copyToVectorData();

   gPlotFactory().createPlot( "testTriangleGenerator/PCM" );
   gPlotFactory().drawPcmData( *data );
   gPlotFactory().createGraph( Utils::createRangeReal( 0, dataCombined.size() ), *dataCombined.copyToVectorData() - *data->copyToVectorData(), Qt::blue );

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

   gPlotFactory().createPlot( "testSawtoothGenerator/PCM" );
   gPlotFactory().drawPcmData( *data );
   gPlotFactory().createGraph( Utils::createRangeReal( 0, dataCombined.size() ), *dataCombined.copyToVectorData() - *data->copyToVectorData(), Qt::blue );

   MultiChannelRawPcmData sawtoothMcRpcm( new RawPcmData( dataCombined ) );
   WaveFile::write( "sawtooth.wav", sawtoothMcRpcm );

   WaveAnalysis::FourierTransform ft( samplingInfo, 4096 * 10, WaveAnalysis::RectangularWindowFuncDef(), 0 );
   double* arr = &((*data)[0]);
   WaveAnalysis::FourierSpectrum::Ptr spec = ft.transform( arr );

   gPlotFactory().createPlot( "testSawtoothGenerator/FT" );
   gPlotFactory().createGraph( spec->getFrequencies(), spec->getMagnitude() );

   if ( checkTodos )
   {
      assert( !"Enable code below" );
   }
   // WaveAnalysis::StftAlgorithm stftAlg( samplingInfo, 4096, WaveAnalysis::RectangularWindowFuncDef(), 4096, 2 );
   // stftAlg.execute( dataCombined );
   // Visualisation::StftGraph stftGraph( stftAlg );
   // stftGraph.create();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testSpectralReassignment
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testSpectralReassignment()
{
   Logger msg( "testSpectralReassignment" );
   msg << Msg::Info << "Running testSpectralReassignment..." << Msg::EndReq;

   double freq = 2000;

   SamplingInfo samplingInfo( 44100 );
   size_t fourierSize = 4096;

   Synthesizer::SquareGenerator sineGen( samplingInfo );
   sineGen.setAmplitude( 0.5 );
   sineGen.setFrequency( freq );
   RawPcmData::Ptr data = sineGen.generate( 44100 );

   // MultiChannelRawPcmData* waveFile = WaveFile::read( GlobalParameters::getTestDataDir() + "atc_intro.wav" );
   // RawPcmData* data = &waveFile->getChannel( 0 );

   WaveAnalysis::SpectralReassignmentTransform specTrans( samplingInfo, fourierSize, fourierSize*3, 2 );
   WaveAnalysis::StftData::Ptr trans = specTrans.execute( *data );

   const WaveAnalysis::SrSpectrum& specReass = dynamic_cast< const WaveAnalysis::SrSpectrum& >( trans->getSpectrum( 0 ) );
   WaveAnalysis::FourierSpectrum spec( specReass );

   const RealVector& freqCorr = specReass.getFrequencyCorrections();

   gPlotFactory().createPlot( "testSpectralReassignment/FrequencyCorrections" );
   gPlotFactory().createGraph( freqCorr );

   size_t index = 0;
   Utils::getMaxValueAndIndex( spec.getMagnitude(), index );
   double ratio = spec.getFrequency( index ) - freq;
   msg << Msg::Info << "Max bin is off by " << ratio << Msg::EndReq;
   msg << Msg::Info << "Correction given = " << freqCorr[ index ] << Msg::EndReq;
   msg << Msg::Info << "Correction factor on correction = " << ratio / freqCorr[index] << Msg::EndReq;

   gPlotFactory().createPlot( "testSpectralReassignment/Comaprison with Fourier" );
   gPlotFactory().createGraph( spec.getFrequencies(), spec.getMagnitude(), Qt::red );
   gPlotFactory().createScatter( specReass.getFrequencies(), specReass.getMagnitude(), Plotting::MarkerDrawAttr() );

   Visualisation::RebinnedSRGraph graph( *trans );
   graph.create( "testSpectralReassignment/Stft" );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testFindMinima
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testFindMinima()
{
   Logger msg( "testFindMinima" );
   msg << Msg::Info << "In testFindMinima..." << Msg::EndReq;

   TRandom3 rand( 1 );

   size_t nElements = 100;
   RealVector v( nElements );
   for ( size_t i = 0; i < nElements; ++i )
   {
      v[ i ] = rand.Uniform();
      if ( v[ i ] < 0.3 )
      {
         v[ i ] = 0;
      }
      if ( v[ i ] > 0.5 && v[ i ] < 0.7 )
      {
         v[ i ] = 0.5;
      }
      if ( v[ i ] > 0.8 )
      {
         v[ i ] = 1;
      }
   }

   gPlotFactory().createPlot( "testFindMinima/sampleData" );
   gPlotFactory().createGraph( v );
   gPlotFactory().createScatter( Utils::createRangeReal( 0, v.size() ), v );

   std::vector< size_t > result = Utils::findMinima( v );
   for ( size_t i = 0; i < result.size(); ++i )
   {
      msg << Msg::Info << "Min at " << result[ i ] << Msg::EndReq;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testRandomRumberGenerator
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Create two random data sets with same seed. Check if both data sets are identical.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testRandomNumberGenerator()
{
   Logger msg( "testRandomNumberGenerator" );
   msg << Msg::Info << "In testRandomNumberGenerator..." << Msg::EndReq;

   size_t numSamples = 100;

   RealVector set1( numSamples );
   RandomNumberGenerator rng1( 1 );
   for ( size_t i = 0; i < numSamples; ++i )
   {
      set1[ i ] = rng1.uniform();
   }

   RealVector set2( numSamples );
   RandomNumberGenerator rng2( 1 );
   for ( size_t i = 0; i < numSamples; ++i )
   {
      set2[ i ] = rng2.uniform();
   }

   for ( size_t i = 0; i < set1.size(); ++i )
   {
      msg << Msg::Info << "set1[ " << i << " ] = " << set1[ i ] << ", set2[ " << i << " ] = " << set2[ i ] << Msg::EndReq;
      /// Double comparison possible.
      if ( set1[ i ] - set2[ i ] != 0 )
      {
         throw ExceptionTestFailed( "testRandomNumberGenerator", "Irreproducible random numbers." );
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testHist2D
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testHist2D()
{
   Logger msg( "testHist2D" );
   msg << Msg::Info << "In testHist2D..." << Msg::EndReq;

   Math::Regular2DHistogram hist2D( 250, -5, 5, 250, -5, 5 );

   RandomNumberGenerator rng( 1 );
   size_t nSamples = 10000000;

   for ( size_t i = 0; i < nSamples; ++i )
   {
      double x = rng.gauss( 0, 1 );
      double y = rng.gauss( 2, 1 );
      hist2D.add( x, y, 1 );
   }

   msg << Msg::Debug << "Maximum value = " << hist2D.getMaximum() << Msg::EndReq;
   msg << Msg::Debug << "Minimum value = " << hist2D.getMinimum() << Msg::EndReq;

   gPlotFactory().createPlot( "testHist2D/gaussianSampling" );
   gPlotFactory().create2DHist( hist2D );

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testHistogram
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestSuite::testHistogram()
{
   Logger msg( "testHistogram" );
   msg << Msg::Info << "In testHistogram..." << Msg::EndReq;

   RandomNumberGenerator rand( 1 );

   size_t nBins = 100;
   double xMin = -5;
   double xMax = 5;

   Math::RegularAccumArray hist( nBins, xMin, xMax );
   size_t nDraws = 5000;
   double w = ( xMax - xMin ) / nDraws;

   for ( size_t i = 0; i < nDraws; ++i )
   {
      double x = rand.gauss( 0, 1 );
      hist.add( x, w );
   }

   Math::GaussPdf pdf( 0, 1 );
   const RealVector& xEval = Utils::createRangeReal( xMin, xMax, 100 );
   Math::RealMemFunction<Math::GaussPdf> pdfFunc( &Math::GaussPdf::getDensity, &pdf );
   const RealVector& yEval = pdfFunc.evalMany( xEval );

   gPlotFactory().createPlot( "testHistogram/Gauss sampling" );
   gPlotFactory().createHistogram( hist );
   gPlotFactory().createGraph( xEval, yEval, Qt::blue );
}

