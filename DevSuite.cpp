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
   // devSidelobeSubtraction(); /// parked
   // devPeakFinder();
   // devFourierPeakFinder1();
   // devFourierPeakFinder2();
   // devTwoTuple();
   // devRebinSRSpec();
   devPeakFinder2();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Include section
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <set>
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

void sortVectors( RealVector& freqVec, RealVector& magVec )
{
   std::map< double, int > sortIndices;
   for ( size_t i = 0; i < freqVec.size(); ++i )
   {
      sortIndices[ freqVec[i] ] = i;
   }

   RealVector magVecNew( magVec.size() );
   size_t i = 0;
   for ( std::map< double, int >::const_iterator it = sortIndices.begin(); it != sortIndices.end(); ++it, ++i )
   {
      freqVec[ i ] = it->first;
      magVecNew[ i ] = magVec[ it->second ];
   }
   magVec = magVecNew;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devFourierPeakFinder
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::devFourierPeakFinder1()
{
   SamplingInfo samplingInfo;

   Synthesizer::SquareGenerator square( samplingInfo );
   square.setFrequency( 440 );
   RawPcmData::Ptr data = square.generate( 44100 );
   square.setFrequency( 900 );
   RawPcmData::Ptr data2 = square.generate( 44100 );
   // data->mixAdd( *data2 );

   size_t fourierSize = 1024;
   WaveAnalysis::SpectralReassignmentTransform transform( samplingInfo, fourierSize, 0, 2 );
   WaveAnalysis::RawStftData::Ptr stftData = transform.execute( *data );

   WaveAnalysis::SRSpectrum& spec = static_cast< WaveAnalysis::SRSpectrum& >( stftData->getSpectrum( 0 ) );
   RealVector freqVec = spec.getFrequencies();
   RealVector magVec = spec.getMagnitude();

   sortVectors( freqVec, magVec );

   new TCanvas();
   TGraph* gr = RootUtilities::createGraph( freqVec, magVec );
   gr->Draw( "AP" );
   gr->SetMarkerStyle( 21 );
   gr->SetMarkerSize( 0.5 );

   size_t maxIndex;
   size_t nIter = 0;
   size_t maxIter = 100;

   size_t nNbSearchRad = 5;
   RealVector peaks;
   RealVector peakMass;

   while ( nIter < maxIter )
   {
      std::cout << "====== ITERATION " << nIter << " =====" << std::endl;
      double maxVal = Utils::getMaxValueAndIndex( magVec, maxIndex );

      std::cout << "Frequency @ max = " << freqVec[maxIndex] << std::endl;

      RealVector distAddCand;
      RealVector magAddCand;

      size_t iMin = std::max( 0, static_cast< int >( maxIndex - nNbSearchRad ) );
      size_t iMax = std::min( magVec.size() - 1, maxIndex + nNbSearchRad );

      RealVector neighbourIndices;

      for ( size_t i = iMin; i <= iMax; ++i )
      {
         if ( i != maxIndex )
         {
            distAddCand.push_back( freqVec[i] - freqVec[maxIndex] );
            magAddCand.push_back( magVec[i] );
            neighbourIndices.push_back( i );
            std::cout << "relIndex = " << static_cast< int >( i - maxIndex ) << ": dist = " << distAddCand.back() << ", mag = " << magAddCand.back() << std::endl;
         }
      }

      std::set< size_t > pointsAdded;
      pointsAdded.insert( maxIndex );

      double volume = 0;
      double mass = maxVal;

      size_t iDensIter = 0;
      bool densHasImproved = true;
      double density = 0;
      while ( densHasImproved )
      {
         double highestNewDens = 0;
         int iPointToAdd = -1;
         RealVector::iterator itDist = distAddCand.begin();
         RealVector::iterator itMag = magAddCand.begin();
         while ( itDist != distAddCand.end() )
         {
            double newMass = mass + *itMag;
            double newVol = volume + fabs( *itDist );
            double newDens = newMass / newVol;
            std::cout << "*itDist = " << *itDist << std::endl;
            std::cout << "newDens candidate = " << newDens << std::endl;
            if ( newDens > highestNewDens )
            {
               highestNewDens = newDens;
               iPointToAdd = itDist - distAddCand.begin();
            }
            ++itDist;
            ++itMag;
         }

         if ( iPointToAdd == -1 )
         {
            std::cout << "All points are assigned => break" << std::endl;
            break;
         }

         if ( highestNewDens > density || iDensIter < 2 )
         {
            density = highestNewDens;
            std::cout << "New dens = " << density << std::endl;
            volume += distAddCand[ iPointToAdd ];
            mass += magAddCand[ iPointToAdd ];
            pointsAdded.insert( *( neighbourIndices.begin() + iPointToAdd ) );

            distAddCand.erase( distAddCand.begin() + iPointToAdd );
            magAddCand.erase( magAddCand.begin() + iPointToAdd );
            neighbourIndices.erase( neighbourIndices.begin() + iPointToAdd );
         }
         else
         {
            densHasImproved = false;
            std::cout << "Density did not improve" << std::endl;
         }

         ++iDensIter;
      } /// inner while loop

      peaks.push_back( freqVec[maxIndex] );
      peakMass.push_back( mass );

      for ( std::set< size_t >::reverse_iterator it = pointsAdded.rbegin(); it != pointsAdded.rend(); ++it )
      {
         std::cout << "Removing point " << *it << std::endl;
         magVec.erase( magVec.begin() + *it );
         freqVec.erase( freqVec.begin() + *it );
      }

      ++nIter;
   }

   TGraph* grRemoved = RootUtilities::createGraph( freqVec, magVec );
   grRemoved->SetMarkerSize( 1 );
   grRemoved->SetMarkerColor( kRed );
   grRemoved->Draw( "PSAME" );

   for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
   {
      TLine* line = new TLine( peaks[iPeak], 0, peaks[iPeak], peakMass[iPeak] );
      line->Draw();
   }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Peakfinder development
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "TRandom3.h"


void DevSuite::devPeakFinder()
{
   const RealVector& dataSet = TestDataSupply::createNoiseAndPeaks();

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devFourierPeakFinder2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::devFourierPeakFinder2()
{
   SamplingInfo samplingInfo;

   Synthesizer::SquareGenerator square( samplingInfo );
   square.setFrequency( 440 );
   RawPcmData::Ptr data = square.generate( 44100 );
   square.setFrequency( 900 );
   RawPcmData::Ptr data2 = square.generate( 44100 );
   // data->mixAdd( *data2 );

   size_t fourierSize = 1024;
   WaveAnalysis::SpectralReassignmentTransform transform( samplingInfo, fourierSize, 0, 2 );
   WaveAnalysis::RawStftData::Ptr stftData = transform.execute( *data );

   WaveAnalysis::SRSpectrum& spec = static_cast< WaveAnalysis::SRSpectrum& >( stftData->getSpectrum( 0 ) );
   RealVector freqVec = spec.getFrequencies();
   RealVector magVec = spec.getMagnitude();

   sortVectors( freqVec, magVec );

   new TCanvas();
   TGraph* gr = RootUtilities::createGraph( freqVec, magVec );
   gr->Draw( "AP" );
   gr->SetMarkerStyle( 21 );
   gr->SetMarkerSize( 0.5 );

   // const RealVector& vMovAvg =
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devTwoTuple
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::devTwoTuple()
{
   Logger msg( "devTwoTuple" );
   msg << Msg::Info << "In devTwoTuple..." << Msg::EndReq;

   SamplingInfo samplingInfo;

   Synthesizer::SquareGenerator square( samplingInfo );
   square.setAmplitude( 1 );
   square.setFrequency( 440 );
   RawPcmData::Ptr data = square.generate( 44100 );
   square.setFrequency( 900 );
   RawPcmData::Ptr data2 = square.generate( 44100 );
   data->mixAdd( *data2 );

   size_t fourierSize = 1024;
   WaveAnalysis::SpectralReassignmentTransform transform( samplingInfo, fourierSize, 0, 2 );
   WaveAnalysis::RawStftData::Ptr stftData = transform.execute( *data );

   WaveAnalysis::SRSpectrum& spec = static_cast< WaveAnalysis::SRSpectrum& >( stftData->getSpectrum( 0 ) );
   const RealVector& freqVec = spec.getFrequencies();
   const RealVector& magVec = spec.getMagnitude();

   Math::TwoTuple tuple( freqVec, magVec );
   SortCache sortX( tuple.getX() );

   const RealVector& xSorted = sortX.applyTo( tuple.getX() );
   const RealVector& ySorted = sortX.applyTo( tuple.getY() );

   for ( size_t i = 0; i < tuple.getNumElements(); ++i )
   {
      msg << Msg::Verbose << xSorted[ i ] << ", " << ySorted[ i ] << Msg::EndReq;
   }

   TGraph* gr = RootUtilities::createGraph( xSorted, ySorted );
   gr->Draw( "AL" );
}

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
   WaveAnalysis::RawStftData::Ptr stftData = transform.execute( *data );

   WaveAnalysis::SRSpectrum& spec = static_cast< WaveAnalysis::SRSpectrum& >( stftData->getSpectrum( monitorIndex ) );

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
   WaveAnalysis::RawStftData::Ptr normalTrans = normalTransAlg.execute( *data );
   const WaveAnalysis::FourierSpectrum& normalSpec = normalTrans->getSpectrum( 5 );
   // TGraph* grNormal = RootUtilities::createGraph( normalSpec.getFrequencies(), normalSpec.getMagnitude() );
   // grNormal->Draw( "LSAME" );
   // grNormal->SetLineColor( kBlue );

   // Visualisation::RebinnedSRGraph rebinnedGraph( *stftData, stftData->getNumSpectra(), 0 );
   // rebinnedGraph.create();

   const WaveAnalysis::SRSpectrum& testSpec = dynamic_cast< WaveAnalysis::SRSpectrum& >( stftData->getSpectrum( monitorIndex ) );

   FeatureAlgorithm::GroundtoneHypothesisBuilder gtSeed( testSpec );
   const RealVector& vec = gtSeed.execute( 20 );
   msg << Msg::Info << vec << Msg::EndReq;

}

void DevSuite::devPeakFinder2()
{
   Logger msg( "devPeakFinder2" );
   msg << Msg::Info << "Running devPeakFinder2..." << Msg::EndReq;

   // const Math::RegularAccumArray& data = TestDataSupply::drawNoiseAndPeaks();
   size_t specIndex = 0;
   WaveAnalysis::RawStftData::Ptr stftData = TestDataSupply::getSrFtData();
   const WaveAnalysis::SRSpectrum& spec = dynamic_cast< WaveAnalysis::SRSpectrum& >( stftData->getSpectrum( specIndex ) );
   const Math::RegularAccumArray& data = spec.rebinToFourierLattice();

   // TH1F* hist = data.createHistogram();
   // hist->Draw();

   FeatureAlgorithm::AccumArrayPeakAlgorithm peakAlg;
   peakAlg.execute( data );

   return;
}

