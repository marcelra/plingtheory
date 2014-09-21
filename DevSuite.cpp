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

   devIterateSrPeaks();

   return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Include section
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "SrSpecPeakAlgorithm.h"
#include "RebinnedSrGraph.h"
#include "SpectralReassignmentTransform.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devIterateSrPeaks
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::devIterateSrPeaks()
{
   Logger msg( "devIterateSrPeaks" );
   msg << Msg::Info << "Running devIterateSrPeaks" << Msg::EndReq;

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
      msg << Msg::Info << "iHop = " << iHop << Msg::EndReq;
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

         msg << Msg::Info << "Peak height = " << peak.getHeight() << Msg::EndReq;
      }
   }
   return;
}
