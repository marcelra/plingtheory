#include "DevSuite.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// execute
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::execute()
{
   devSpectralReassignment();
   // devSidelobeSubtraction();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Include section
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Logger.h"
#include "RawPcmData.h"
#include "FourierSpectrum.h"
#include "FourierTransform.h"
#include "SineGenerator.h"
#include "SquareGenerator.h"
#include "RootUtilities.h"
#include "Utils.h"
#include "SpectralReassignmentTransform.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devSpectralReassignment
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::devSpectralReassignment()
{
   Logger msg( "devSpectralReassignment" );
   msg << Msg::Info << "Running devSpectralReassignment..." << Msg::EndReq;

   double freq = 2000;

   SamplingInfo samplingInfo( 44100 );
   size_t fourierSize = 1024;

   Synthesizer::SquareGenerator sineGen( samplingInfo );
   sineGen.setAmplitude( 0.5 );
   sineGen.setFrequency( freq );
   RawPcmData::Ptr data = sineGen.generate( 44100 );

   WaveAnalysis::SpectralReassignmentTransform specTrans( samplingInfo, fourierSize, 0, 2 );
   WaveAnalysis::RawStftData::Ptr trans = specTrans.execute( *data );

   const WaveAnalysis::SRSpectrum& specReass = dynamic_cast< const WaveAnalysis::SRSpectrum& >( trans->getSpectrum( 0 ) );
   WaveAnalysis::FourierSpectrum spec( specReass );

   const RealVector& freqCorr = specReass.getFrequencyCorrections();
   new TCanvas();
   TGraph* grCorr = RootUtilities::createGraph( freqCorr );
   grCorr->Draw( "AL" );

   size_t index = 0;
   Utils::getMaxValueAndIndex( spec.getMagnitude(), index );
   double ratio = spec.getFrequency( index ) - freq;
   msg << Msg::Info << "Max bin is off by " << ratio << Msg::EndReq;
   msg << Msg::Info << "Correction given = " << freqCorr[ index ] << Msg::EndReq;
   msg << Msg::Info << "Correction factor on correction = " << ratio / freqCorr[index] << Msg::EndReq;

   TGraph* gr = RootUtilities::createGraph( spec.getFrequencies(), spec.getMagnitude() );
   TGraph* grReass = RootUtilities::createGraph( specReass.getFrequencies(), specReass.getMagnitude() );

   new TCanvas();
   grReass->Draw( "AP" );
   gr->SetLineColor( kRed );
   gr->Draw( "LSAME" );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devSidelobeSubtraction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::devSidelobeSubtraction()
{
   Logger msg( "devSidelobeSubtraction" );
   msg << Msg::Info << "Running devSidelobeSubtraction..." << Msg::EndReq;

   SamplingInfo samplingInfo( 44100 );
   size_t fourierSize = 2048;

   Synthesizer::SineGenerator sineGen( samplingInfo );
   sineGen.setAmplitude( 0.5 );
   sineGen.setFrequency( 440 );
   RawPcmData::Ptr data = sineGen.generate( 44100 );

   WaveAnalysis::FourierTransform ft( samplingInfo, fourierSize, WaveAnalysis::HanningWindowFuncDef(), 3*fourierSize );
   WaveAnalysis::FourierTransform ftTRamp( samplingInfo, fourierSize, WaveAnalysis::HanningTimeRampedWindowFuncDef(), 3*fourierSize );
   WaveAnalysis::FourierTransform ftTDeriv( samplingInfo, fourierSize, WaveAnalysis::HanningDerivativeWindowFuncDef(), 3*fourierSize );

   WaveAnalysis::FourierSpectrum::Ptr spec = ft.transform( &(*data)[0] );
   WaveAnalysis::FourierSpectrum::Ptr specTRamp = ftTRamp.transform( &(*data)[0] );
   WaveAnalysis::FourierSpectrum::Ptr specTDeriv = ftTDeriv.transform( &(*data)[0] );

   TGraph* grRaw = RootUtilities::createGraph( spec->getFrequencies(), spec->getMagnitude() );

   size_t maxPos = 0;
   Utils::getMaxValueAndIndex( spec->getMagnitude(), maxPos );
   msg << Msg::Verbose << "Max is positioned at " << maxPos << Msg::EndReq;
   double omegaHat = -( (*specTDeriv)[maxPos] * conj( (*spec)[maxPos] ) / (*spec)[maxPos]*(*spec)[maxPos] ).imag();
   msg << Msg::Info << "omegaHat = " << omegaHat << Msg::EndReq;

   double detectedFreq = spec->getFrequencies()[maxPos] + omegaHat;
   double detectedAmp = spec->getMagnitudeInBin( maxPos );
   double detectedPhase = spec->getPhaseInBin( maxPos );


   sineGen.setAmplitude( detectedAmp / fourierSize  * 3.5 );
   sineGen.setPhase( detectedPhase );
   sineGen.setFrequency( detectedFreq );

   msg << Msg::Info << "Detected frequency = " << detectedFreq << Msg::EndReq;
   msg << Msg::Info << "Detected amp = " << detectedAmp << Msg::EndReq;
   msg << Msg::Info << "Detected phase = " << detectedPhase << Msg::EndReq;

   RawPcmData::Ptr reduceFreqData = sineGen.generate( fourierSize );
   WaveAnalysis::FourierSpectrum::Ptr reduceFreqSpec = ft.transform( &(*reduceFreqData)[0] );
   TGraph* grReduce = RootUtilities::createGraph( reduceFreqSpec->getFrequencies(), reduceFreqSpec->getMagnitude() );

   new TCanvas();
   grRaw->Draw( "AL" );
   grReduce->SetLineColor( kRed );
   grReduce->Draw( "LSAME" );

   RealVector specMag = spec->getMagnitude();
   RealVector specRemovedMag = reduceFreqSpec->getMagnitude();
   TGraph* grFlat = RootUtilities::createGraph( spec->getFrequencies(), specMag - specRemovedMag );

   new TCanvas();
   grFlat->Draw( "AL" );



}
