#include "DevSuite.h"
#include "Logger.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// execute
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::execute()
{
   Logger msg( "DevSuite" );
   msg << Msg::Info << "Running development code..." << Msg::EndReq;
   // devSidelobeSubtraction(); /// parked
   devPeakFinder();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Include section
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

#include "TRandom3.h"

RealVector createDataSet( RealVector peakLocs, RealVector peakSigmas, RealVector peakAmpsAtMax )
{
   TRandom3 rand;
   size_t nSamples = 1000;
   double noiseAmp = 5;
   double tilt = 0;

   RealVector dataSet( nSamples );

   double tiltStep = tilt / nSamples;
   for ( size_t i = 0; i < nSamples; ++i )
   {
      // double noise = noiseAmp * rand.Uniform();
      double noise = rand.Gaus( 0, noiseAmp );
      dataSet[i] = noise + tiltStep * i;
   }

   size_t nPeaks = peakLocs.size();

   for ( size_t iPeak = 0; iPeak < nPeaks; ++iPeak )
   {
      double norm = peakAmpsAtMax[iPeak];
      double alpha = 1 / ( peakSigmas[iPeak] * peakSigmas[iPeak] * 2 );
      for ( size_t iSample = 0; iSample < nSamples; ++iSample )
      {
         double deltaX = iSample - peakLocs[iPeak];
         double gaussVal = norm * exp( -deltaX * deltaX * alpha );
         dataSet[iSample] += gaussVal;
      }
   }
   return dataSet;
}

void DevSuite::devPeakFinder()
{
   const double peakLocArr[] = { 100, 300, 600, 750 };
   const double peakSigmaArr[] = { 10, 2, 10, 5};
   const double peakAmpArr[] = { -10, -5, 10, -20 };
   size_t nPeaks = sizeof( peakLocArr ) / sizeof( double );
   // nPeaks = 0;

   RealVector peakLocs( &peakLocArr[0], &peakLocArr[0] + nPeaks );
   RealVector peakSigmas( &peakSigmaArr[0], &peakSigmaArr[0] + nPeaks );
   RealVector peakAmpsAtMax( &peakAmpArr[0], &peakAmpArr[0] + nPeaks );

   const RealVector& dataSet = createDataSet( peakLocs, peakSigmas, peakAmpsAtMax );

   RealVector xArr;
   for ( size_t i = 0; i < dataSet.size(); ++i )
   {
      xArr.push_back( i );
   }

   new TCanvas();
   TGraph* gr = RootUtilities::createGraph( xArr, dataSet );
   gr->Draw( "AL" );

   size_t nBins = 50;
   double maxVal = Utils::getMaxValue( dataSet );
   double minVal = Utils::getMinValue( dataSet );
   TH1F* h1 = new TH1F( "h1", "h1", nBins, minVal, maxVal);
   for ( size_t iSample = 0; iSample < dataSet.size(); ++iSample )
   {
      h1->Fill( dataSet[iSample] );
   }

   new TCanvas();
   h1->Scale( 1 / h1->Integral() );
   h1->Draw();

   RealVector localLhArr( xArr.size() );
   RealVector movAvg( xArr.size() );

   size_t nSamplesMovAvg = 20;
   for ( size_t iSample = 0; iSample < dataSet.size(); ++iSample )
   {
      double avg = 0;
      double localLh = 0;
      size_t iLeft = std::max( 0, static_cast< int > ( iSample - nSamplesMovAvg / 2 ) );
      size_t iRight = std::min( dataSet.size(), iSample + nSamplesMovAvg / 2 );
      for ( size_t i = iLeft; i < iRight; ++i )
      {
         avg += dataSet[i];
         size_t lhBin = h1->GetXaxis()->FindBin( dataSet[i] );
         double p = h1->GetBinContent( lhBin );
         localLh += -p * log( p );
      }
      movAvg[iSample] = avg / nSamplesMovAvg;
      localLhArr[iSample] = localLh / ( iRight - iLeft );
   }
//
   // TGraph* grMovAvg = RootUtilities::createGraph( xArr, movAvg );
   // grMovAvg->SetLineColor( kBlue );
   // grMovAvg->Draw( "LSAME" );

   TCanvas* c = new TCanvas();
   c->Divide( 1, 2 );
   c->cd( 2 );
   gr->Draw( "AL" );
   c->cd( 1 );
   TGraph* grLh = RootUtilities::createGraph( xArr, localLhArr );
   grLh->Draw( "AL ");

   RealVector localLhCopy = localLhArr;
   std::sort( localLhCopy.begin(), localLhCopy.end() );

   new TCanvas();
   RootUtilities::createGraph( xArr, localLhCopy )->Draw( "AL" );


}


