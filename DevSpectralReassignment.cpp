#include "DevSpectralReassignment.h"

#include "Logger.h"
#include "SineGenerator.h"
#include "RawPcmData.h"
#include "RootUtilities.h"
#include "WindowFunction.h"
#include "FftwAlgorithm.h"
#include "NaivePeaks.h"
#include "Peak.h"

#include <iostream>
#include <cmath>

namespace
{

RawPcmData::Ptr generateSineTestSample( double frequency )
{
   SamplingInfo samplingInfo( 44100 );
   Synthesizer::SineGenerator sinGen( samplingInfo );
   sinGen.setFrequency( frequency );
   sinGen.setAmplitude( 0.5 );
   sinGen.setPhase( 0 );

   return sinGen.generate( 44100 );
}

RawPcmData::Ptr generateChirpTestSample( double frequency, size_t offset, size_t length )
{
   SamplingInfo samplingInfo( 44100 );
   RawPcmData* result = new RawPcmData( samplingInfo, offset, 0 );
   Synthesizer::SineGenerator sinGen( samplingInfo );
   double period = samplingInfo.getPeriodInSamples( frequency );
   sinGen.setFrequency( frequency );
   sinGen.setAmplitude( 0.5 );
   sinGen.setPhase( 0 );
   /// Modify length to a multiple number of periods
   size_t nPeriods = length / period;
   length = nPeriods * period;
   result->pasteAtEnd( *sinGen.generate( length ) );
   RawPcmData end( SamplingInfo(), 44100 - offset - length, 0 );
   result->pasteAtEnd( end );
   return RawPcmData::Ptr( result );
}

}

namespace Dev
{

DevSpectralReassignment::DevSpectralReassignment()
{
}

void testWindowFunctions()
{
   size_t fourierLength = 2048;
   WaveAnalysis::HanningWindowFunction hann( fourierLength );
   WaveAnalysis::HanningDerivativeWindowFunction hannDeriv( fourierLength );
   WaveAnalysis::HanningTimeRampedWindowFunction hannTimeRamp( fourierLength );

   RealVector window;
   RealVector windowDeriv;
   RealVector windowTimeRamp;
   for ( size_t i = 0; i < fourierLength; ++i )
   {
      window.push_back( hann.calc( i ) );
      windowDeriv.push_back( hannDeriv.calc( i ) );
      windowTimeRamp.push_back( hannTimeRamp.calc( i ) );
   }

   TCanvas* canWindow = new TCanvas( "windowFunction", "windowFunction", 1000, 400 );
   canWindow->Divide( 3, 0 );

   canWindow->cd( 1 );
   TGraph* grWindow = RootUtilities::createGraph( window );
   grWindow->Draw( "AL" );

   canWindow->cd( 2 );
   TGraph* grWindowDeriv = RootUtilities::createGraph( windowDeriv );
   grWindowDeriv->Draw( "AL" );

   canWindow->cd( 3 );
   TGraph* grWindowTimeRamp = RootUtilities::createGraph( windowTimeRamp );
   grWindowTimeRamp->Draw( "AL" );
}

RealVector applyWindowTo( const RawPcmData& data, size_t offset, const WaveAnalysis::WindowFunction& window, size_t zeroPadding )
{
   RealVector result( window.getSize() * ( 1 + 2 * zeroPadding ), 0 );
   size_t iResultIndex = zeroPadding * window.getSize();
   for ( size_t i = offset; i < window.getSize(); ++i, ++iResultIndex )
   {
      result[iResultIndex] = window.calc(i) * data[i];
      // std::cout << "windowed data = " << result[iResultIndex] << std::endl;
   }
   // std::cout << result.size() << std::endl;
   return result;
}

RealVector undoPadding( const RealVector& vec, size_t padding )
{
   RealVector result( &vec[padding], &vec[vec.size() - 1 - padding] );
   return result;
}

typedef std::vector< std::complex< double > > ComplexVector;

ComplexVector transform( const RealVector& data, WaveAnalysis::FftwAlgorithm& alg )
{
   const std::complex< double >* ftArr = alg.transform( &data[0] );
   std::vector< std::complex< double > > ft( ftArr, ftArr + alg.getSpectrumDimension() );
   return ft;
}

// void removeAndRedo()
// {
//    Logger msg( "DevSpectralReassignment" );
//    msg << Msg::Info << "Running the removeAndRedo test" << Msg::EndReq;
//
//    size_t zeroPadding = 32;
//    size_t fourierSize = 256;
//    RawPcmData::Ptr testSample = generateSineTestSample( 44100. / (fourierSize) * 40.00 );
//
//    WaveAnalysis::AdvancedFourierTransform ft( testSample->getSamplingInfo(), fourierSize * ( 1 + 2 * zeroPadding ) );
//
//    WaveAnalysis::HanningWindowFunction rectWindow( fourierSize );
//
//    const RealVector& windowedData = applyWindowTo( *testSample, 0, rectWindow, zeroPadding );
//
//    ComplexVector ft = transform( windowedData, fftwAlg );
//    WaveAnalysis::FourierSpectrum spec( testSample->getSamplingInfo(), &ft[0], &ft[0] + ft.size() );
//
//    RealVector spectrogram = spec.getMagnitude();
//    RealVector spectrogramOrig = spec.getMagnitude();
//    FeatureAlgorithm::NaivePeaks naivePeaks( spectrogram, FeatureAlgorithm::NaivePeaks::Max, 1 );
//    naivePeaks.execute();
//    size_t peakPos = naivePeaks.getPeak( 0 ).getPositionIndex();
//
//    std::complex<double> tmp = spec[ peakPos ];
//    for ( size_t i = 0; i < spec.size(); ++ i )
//    {
//       spec[i] = 0;
//    }
//    spec[ peakPos ] = tmp;
//
//    double fac = 1. / sqrt( fftwAlg.getFourierSize() );
//    for ( size_t i = 0; i < spec.size(); ++i )
//    {
//       spec[i] *= fac;
//       spectrogramOrig[i] *= fac;
//    }
//
//    spectrogram = spec.getMagnitude();
//    RootUtilities::createGraph( spectrogram )->Draw( "AL" );
//    TGraph* grOrigSpec = RootUtilities::createGraph( spectrogramOrig );
//    grOrigSpec->SetLineColor( kBlue );
//    grOrigSpec->Draw( "LSAME" );
//
//    RawPcmData::Ptr freqRemovedData = fftwAlg.transform( spec );
//    RealVectorPtr v = freqRemovedData->copyToVectorData();
//    const RealVector& unpaddedData = undoPadding( *v, zeroPadding * fourierSize );
//
//    TGraph* grRemoved = RootUtilities::createGraph( unpaddedData );
//    TGraph* grOriginal = RootUtilities::createGraph( *testSample );
//
//    new TCanvas();
//    grRemoved->Draw( "AL" );
//    grOriginal->Draw( "LSAME" );
//    grRemoved->SetLineColor( kRed );
//
// }
//
// RawPcmData::Ptr generateTestSample( double period, double lfoFreq, size_t numSamples )
// {
//    SamplingInfo samplingInfo( 44100 );
//    RawPcmData* data = new RawPcmData( samplingInfo, numSamples );
//
//    double phaseStep = 2 * M_PI / period; // samplingInfo.getPhaseStepPerSample( frequency );
//    double phaseStepLfo = samplingInfo.getPhaseStepPerSample( lfoFreq );
//    double maxAmp = 1;
//    double phase = 0;
//    double phaseLfo = 0;
//    for ( size_t i = 0; i < numSamples; ++i )
//    {
//       double amp = maxAmp * cos( phaseLfo );
//       double val = amp * sin( phase );
//       phaseLfo += phaseStepLfo;
//       phase += phaseStep;
//       (*data)[i] = val;
//    }
//    return RawPcmData::Ptr( data );
// }
//
// void testTimeComponents()
// {
//    size_t fourierSize = 4096 * 16;
//    RawPcmData::Ptr data = generateTestSample( 4096., 0, fourierSize );
//
//    /// Draw raw data
//    new TCanvas( "data", "data" );
//    TGraph* grData = RootUtilities::createGraph( *data );
//    grData->Draw( "AL" );
//
//    WaveAnalysis::FftwAlgorithm fftwAlg( fourierSize );
//
//    const std::complex<double>* ftArr = fftwAlg.transform( &(*data)[0] );
//    ComplexVector compSpec = ComplexVector( ftArr, ftArr + fftwAlg.getSpectrumDimension() );
//
//    WaveAnalysis::FourierSpectrum spec( data->getSamplingInfo(), ftArr, ftArr + fftwAlg.getSpectrumDimension() );
//
//    size_t maxBin = 0;
//    double maxVal = 0;
//    for ( size_t i = 0; i < spec.size(); ++i )
//    {
//       double val = abs( spec[i] );
//       if ( val > maxVal )
//       {
//          maxVal = val;
//          maxBin = i;
//       }
//    }
//
//    for ( size_t i = 0; i < 1; ++i )
//    {
//       spec[maxBin + i] = 0;
//       spec[maxBin - i] = 0;
//    }
//    // for ( size_t i = 0; i < spec.size(); ++i )
//    // {
//    //    if ( i != maxBin )
//    //    {
//    //       spec[i] = 0;
//    //    }
//    //    else
//    //    {
//    //       spec[i] = spec[i] * std::complex< double >( 1./fourierSize, 0 );
//    //    }
//    // }
//
//    RealVector ftX = spec.getFrequencies();
//    RealVector ftY = spec.getMagnitude();
//
//    new TCanvas( "Spectrum", "Spectrum" );
//    TGraph* grSpec = RootUtilities::createGraph( ftX, ftY );
//    grSpec->Draw( "AL" );
//
//    const double* revTransArr = fftwAlg.reverseTransform( &spec[0] );
//    RawPcmData revTransData( data->getSamplingInfo(), revTransArr, revTransArr + fourierSize );
//
//    new TCanvas( "RevTransform", "RevTransform" );
//    TGraph* grRevTransform = RootUtilities::createGraph( revTransData );
//    grRevTransform->Draw( "AL" );
// }

void test()
{
//    testTimeComponents();
//    // removeAndRedo();
//    return;
//
//    Logger msg( "DevSpectralReassignment" );
//    msg << Msg::Info << "Running tests" << Msg::EndReq;
//
//    testWindowFunctions();
//
//    // RawPcmData::Ptr testSample = generateSineTestSample( 44100 / 2048. * 20.75 );
//    // RawPcmData::Ptr testSample2 = generateSineTestSample( 44100 / 1024. * 20.00 );
//    RawPcmData::Ptr testSample = generateSineTestSample( 400 );
//    //RawPcmData::Ptr testSample2 = generateSineTestSample( 440 * pow( 2., 1./12.) );
//    // testSample->mixAdd( *testSample2, 0 );
//    // RawPcmData::Ptr testSample = generateChirpTestSample( 400, 5000, 1000 );
//
//    new TCanvas( "canSample", "canSample" );
//    TGraph* grData = RootUtilities::createGraph( *testSample );
//    grData->Draw( "AL" );
//
//    size_t zeroPadding = 2;
//    size_t fourierSize = 1024;
//    WaveAnalysis::FftwAlgorithm fftwAlg( fourierSize * ( 1 + 2 * zeroPadding ) );
//
//    WaveAnalysis::HanningWindowFunction hann( fourierSize );
//    WaveAnalysis::HanningDerivativeWindowFunction hannDeriv( fourierSize );
//    WaveAnalysis::HanningTimeRampedWindowFunction hannTimeRamp( fourierSize );
//
//    const RealVector& window = applyWindowTo( *testSample, 0, hann, zeroPadding );
//    const RealVector& windowDeriv = applyWindowTo( *testSample, 0, hannDeriv, zeroPadding );
//    const RealVector& windowTimeRamped = applyWindowTo( *testSample, 0, hannTimeRamp, zeroPadding );
//
//    WaveAnalysis::RectangularWindowFunction square( fourierSize );
//    WaveAnalysis::HannPoissonWindowFunction hannPois( fourierSize );
//
//    const RealVector& sqWindow = applyWindowTo( *testSample, 0, square, zeroPadding );
//    const RealVector& hpWindow = applyWindowTo( *testSample, 0, hannPois, zeroPadding );
//
//    new TCanvas( "canWindowedData", "canWindowedData" );
//    TGraph* grWindowedData = RootUtilities::createGraph( window );
//    grWindowedData->Draw( "AL" );
//
//    std::cout << "Transforming..." << std::endl;
//    ComplexVector ftSq = transform( sqWindow, fftwAlg );
//    ComplexVector ftHp = transform( hpWindow, fftwAlg );
//    ComplexVector ftHann = transform( window, fftwAlg );
//    ComplexVector ftHannD = transform( windowDeriv, fftwAlg );
//    ComplexVector ftHannTR = transform( windowTimeRamped, fftwAlg );
//    std::cout << "Done." << std::endl;
//
//    RealVector spec1( ftSq.size() );
//    RealVector spec2( ftSq.size() );
//    for ( size_t i = 0; i < ftSq.size(); ++i )
//    {
//       spec1[i] = abs ( ftHann[i] );
//       spec2[i] = abs ( ftSq[i] );
//    }
//
//    new TCanvas( "canSpecTest", "canSpecTest" );
//    TGraph* grSpec1 = RootUtilities::createGraph( spec1 );
//    TGraph* grSpec2 = RootUtilities::createGraph( spec2 );
//    grSpec1->Draw( "AL " );
//    grSpec2->Draw( "LSAME" );
//    grSpec2->SetLineColor( kRed );
//
//    RealVector specX( ftHann.size() );
//    RealVector omegaHat( ftHann.size() );
//    RealVector tHat( ftHann.size() );
//    RealVector specHann( ftHann.size() );
//    RealVector specHannMod( ftHann.size() );
//    for ( size_t i = 0; i < ftHann.size(); ++i )
//    {
//       tHat[i] = ( ftHannTR[i] * conj( ftHann[i] ) / abs( ftHann[i] ) ).real();
//       omegaHat[i] = ( ftHannD[i] * conj( ftHann[i] ) / abs( ftHann[i] ) ).imag();
//       std::cout << "i = " << i << ", omegaHat = " << omegaHat[i] << ", tHat = " << tHat[i] << std::endl;
//       specX[i] = i + omegaHat[i];
//       specHann[i] =  abs( ftHann[i] );
//       specHannMod[i] = ( 1 - 0*fabs( omegaHat[i] ) ) * abs( ftHann[i] ) / fabs( tHat[i] ); // / ( fabs(tHat[i]) + 1 );
//    }
//
//    new TCanvas( "canOmegaHat", "canOmegaHat" );
//    TGraph* grOmegaHat = RootUtilities::createGraph( omegaHat );
//    grOmegaHat->Draw( "AL" );
//
//    new TCanvas( "canTHat", "canTHat" );
//    TGraph* grTHat = RootUtilities::createGraph( tHat );
//    grTHat->Draw(" AL" );
//
//    new TCanvas( "canSpecReass", "canSpecReass" );
//    TGraph* grSpecTest = RootUtilities::createGraph( specHann );
//    TGraph* grSpecReass = RootUtilities::createGraph( specX, specHannMod );
//    grSpecReass->Draw( "AL" );
//    grSpecTest->SetLineColor( kRed );
//    grSpecTest->Draw( "LSAME" );
//
//    RealVector v( ftHann.size() );
//    for ( size_t i = 0; i < ftHann.size(); ++i )
//    {
//       v[i] = tHat[i] * omegaHat[i] + specHann[i];
//    }
//
//    RootUtilities::createGraph( v )->Draw( "AL" );
}

} /// namespace Dev
