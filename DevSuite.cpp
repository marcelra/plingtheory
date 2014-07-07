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

   devPeakFinder();

   return;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Include section
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "AccumArrayPeakAlgorithm.h"
#include "GroundtoneHypothesisBuilder.h"
#include "IPlotFactory.h"
#include "NoteList.h"
#include "RootUtilities.h"
#include "SineGenerator.h"
#include "SortCache.h"
#include "SpectralReassignmentTransform.h"

#include "TCanvas.h"
#include "TGraph.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devPeakFinder
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::devPeakFinder()
{
   Logger msg( "devPeakFinder" );
   msg << Msg::Info << "Running devPeakFinder..." << Msg::EndReq;

   // const Math::RegularAccumArray& data = TestDataSupply::drawNoiseAndPeaks();
   // size_t specIndex = 5;
   // WaveAnalysis::StftData::Ptr stftData = TestDataSupply::getSrFtData();
   // const WaveAnalysis::SrSpectrum& spec = dynamic_cast< WaveAnalysis::SrSpectrum& >( stftData->getSpectrum( specIndex ) );
   // const Math::RegularAccumArray& data = spec.rebinToFourierLattice();

   SamplingInfo samplingInfo;
   Synthesizer::SineGenerator sineGen( samplingInfo );
   sineGen.setFrequency( 440 );
   RawPcmData::Ptr data = sineGen.generate( 44100 );

   size_t fourierSize = 2048;
   WaveAnalysis::SpectralReassignmentTransform trans( samplingInfo, fourierSize, fourierSize, 2 );
   WaveAnalysis::StftData::Ptr stftData = trans.execute( *data );

   FeatureAlgorithm::AccumArrayPeakAlgorithm peakAlg( 0 );
   peakAlg.setDoMonitor( true );
   const Math::RegularAccumArray& accArr = dynamic_cast< WaveAnalysis::SrSpectrum& >( stftData->getSpectrum( 0 ) ).rebinToFourierLattice();
   const std::vector< Feature::Peak>& peaks = peakAlg.execute( accArr );

   std::vector< RealVector > inputDataSet;
   std::vector< RealVector > outputDataSet;

   return;
}

