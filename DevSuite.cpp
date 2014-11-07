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

   devPeakSustainAlgorithm();

   return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Include section
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "PeakSustainAlgorithm.h"
#include "RebinnedSRGraph.h"
#include "SpectralReassignmentTransform.h"
#include "SrSpecPeakAlgorithm.h"


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
      msg << Msg::Info << "Consists of " << sustainedPeaks[ i ]->getAllPeaks().size() << " peaks." << Msg::EndReq;
      for ( size_t iSubPeak = 0; iSubPeak < sustainedPeaks[ i ]->getAllPeaks().size(); ++iSubPeak )
      {
         const Feature::IBasicSpectrumPeak& subPeak = *sustainedPeaks[ i ]->getAllPeaks()[ iSubPeak ];
         msg << Msg::Info << " > A = " << subPeak.getHeight() << ", f = " << subPeak.getFrequency() << Msg::EndReq;
      }
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
