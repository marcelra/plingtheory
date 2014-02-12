#include "TestDataSupply.h"

#include "TRandom3.h"

#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// createNoiseAndPeaks
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVector TestDataSupply::createNoiseAndPeaks()
{
   const double peakLocArr[] = { 100, 300, 600, 750 };
   const double peakSigmaArr[] = { 10, 2, 10, 5};
   const double peakAmpArr[] = { -10, -5, 10, -20 };
   size_t nPeaks = sizeof( peakLocArr ) / sizeof( double );
   nPeaks = 0;

   RealVector peakLocs( &peakLocArr[0], &peakLocArr[0] + nPeaks );
   RealVector peakSigmas( &peakSigmaArr[0], &peakSigmaArr[0] + nPeaks );
   RealVector peakAmpsAtMax( &peakAmpArr[0], &peakAmpArr[0] + nPeaks );

   return createNoiseAndPeaks( peakLocs, peakSigmas, peakAmpsAtMax );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// createNoiseAndPeaks
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVector TestDataSupply::createNoiseAndPeaks( RealVector peakLocs, RealVector peakSigmas, RealVector peakAmpsAtMax )
{
   TRandom3 rand;
   size_t nSamples = 1000;
   double noiseAmp = 5;
   double tilt = 0;

   RealVector dataSet( nSamples );

   double tiltStep = tilt / nSamples;
   for ( size_t i = 0; i < nSamples; ++i )
   {
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
