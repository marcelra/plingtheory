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
   size_t specIndex = 5;
   WaveAnalysis::StftData::Ptr stftData = TestDataSupply::getSrFtData();
   const WaveAnalysis::SrSpectrum& spec = dynamic_cast< WaveAnalysis::SrSpectrum& >( stftData->getSpectrum( specIndex ) );
   const Math::RegularAccumArray& data = spec.rebinToFourierLattice();

   // TH1F* hist = data.createHistogram();
   // hist->Draw();

   FeatureAlgorithm::AccumArrayPeakAlgorithm peakAlg( 0 );
   peakAlg.setDoMonitor( true );
   const std::vector< Feature::Peak>& peaks = peakAlg.execute( data );
   return;

   RealVector prominence( peaks.size() );
   RealVector frequencyBin( peaks.size() );
   RealVector width( peaks.size() );
   for ( size_t i = 0; i < peaks.size(); ++i )
   {
      prominence[ i ] = peaks[ i ].getProminence();
      frequencyBin[ i ] = peaks[ i ].getPosition();
      width[ i ] = peaks[ i ].getWidth();
      msg << Msg::Verbose << "i = " << i << Msg::EndReq;
      msg << Msg::Verbose << "Prominence = " << peaks[ i ].getProminence() << Msg::EndReq;
   }

   new TCanvas();
   TGraph* grPromVsFreq = RootUtilities::createGraph( frequencyBin, prominence );
   grPromVsFreq->Draw( "AP" );
   new TCanvas();
   TGraph* grWidthVsFreq = RootUtilities::createGraph( width, frequencyBin );
   grWidthVsFreq->Draw( "AP" );

   SortCache sortProminence( prominence );
   const RealVector& sortedPeaks = sortProminence.applyReverseTo( frequencyBin );
   const RealVector& sortedProminence = sortProminence.applyReverseTo( prominence );

   for ( size_t i = 0; i < 100; ++i )
   {
      msg << Msg::Info << "Peak at " << sortedPeaks[i] << " with prominence: " << sortedProminence[i] << Msg::EndReq;
   }

   const Music::NoteList& notes = Music::createDiatonicScale( Music::Note( Music::Note::B, 1 ) );
   for ( size_t iNote = 0; iNote < notes.size(); ++iNote )
   {
      msg << Msg::Info << "Note " << notes[ iNote ] << " has frequency " << notes[ iNote ].getFrequency() << Msg::EndReq;
   }


   // new TCanvas();
   // Visualisation::RebinnedSRGraph stftGraph( *stftData );
   // stftGraph.create();

   return;
}

