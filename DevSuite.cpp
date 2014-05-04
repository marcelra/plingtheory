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

   // devHistogram();
   // devRebinSRSpec();
   // devFourierTemplates();
   // devSamples();

   // devPeakFinder2();
   // devMlp();
   return;
   devPeakFinder2();
   // devNewtonSolver1D();

   /// Can move to test functions
   // testPdf();

   /// PARKED
   // devSidelobeSubtraction();
   // devEntropyPeaks();

   /// PARKED AND INTERESTING
   // devFourierPeakFinder1();

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Include section
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <set>
#include <cassert>
#include "Regular2DHistogram.h"
#include "GaussPdf.h"
#include "IObjectiveFunction.h"
#include "McmcOptimiser.h"
#include "NoteList.h"
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
#include "KernelPdf.h"
#include "IPlotFactory.h"
#include "IThread.h"
#include "UniformPdf.h"
#include "RealMemFunction.h"
#include "RealFunctionPtr.h"
#include "IRealFunction.h"
#include "ComposedRealFuncWithDerivative.h"
#include "NewtonSolver1D.h"
#include "MultilayerPerceptron.h"
#include "RandomNumberGenerator.h"
#include "RootMlp.h"

#include <functional>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Parked code fragments
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "DevSuite.parked.cpp"

void DevSuite::devNewtonSolver1D()
{
   Logger msg( "devNewtonSolver1D" );
   msg << Msg::Info << "Running devNewtonSolver1D..." << Msg::EndReq;

   Math::GaussPdf gaussPdf( 0, 1 );
   Math::RealMemFunction< Math::GaussPdf > integral( &Math::GaussPdf::getIntegral, &gaussPdf );
   Math::RealMemFunction< Math::GaussPdf > density( &Math::GaussPdf::getDensity, &gaussPdf );
   Math::ComposedRealFuncWithDerivative pdfAsFunc( integral, density );

   Math::NewtonSolver1D newtonSolver;
   Math::NewtonSolver1D::Result result = newtonSolver.solve( pdfAsFunc, 0.95, 0 );
   msg << Msg::Info << "Solution = " << result.getSolution() << Msg::EndReq;

   return;
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
   WaveAnalysis::StftData::Ptr stftData = transform.execute( *data );

   WaveAnalysis::SrSpectrum& spec = static_cast< WaveAnalysis::SrSpectrum& >( stftData->getSpectrum( monitorIndex ) );

   const Math::RegularAccumArray& accArr = spec.rebinToFourierLattice();

   SortCache sortX( spec.getFrequencies() );

   gPlotFactory().createPlot( "devRebinSRSpec/Rebinned" );
   gPlotFactory().createHistogram( accArr );

   // TGraph* grUnbinned = RootUtilities::createGraph( sortX.applyTo( spec.getFrequencies() ), sortX.applyTo( spec.getMagnitude() ) );
   // grUnbinned->SetLineColor( kRed + 2 );
   // grUnbinned->SetMarkerColor( kRed + 2 );
   // grUnbinned->Draw( "PSAME" );

   WaveAnalysis::StftAlgorithm normalTransAlg( data->getSamplingInfo(), fourierSize, WaveAnalysis::HanningWindowFuncDef(), 0, 2 );
   WaveAnalysis::StftData::Ptr normalTrans = normalTransAlg.execute( *data );
   // const WaveAnalysis::FourierSpectrum& normalSpec = normalTrans->getSpectrum( 5 );
   // TGraph* grNormal = RootUtilities::createGraph( normalSpec.getFrequencies(), normalSpec.getMagnitude() );
   // grNormal->Draw( "LSAME" );
   // grNormal->SetLineColor( kBlue );

   // Visualisation::RebinnedSRGraph rebinnedGraph( *stftData, stftData->getNumSpectra(), 0 );
   // rebinnedGraph.create();

   const WaveAnalysis::SrSpectrum& testSpec = dynamic_cast< WaveAnalysis::SrSpectrum& >( stftData->getSpectrum( monitorIndex ) );

   FeatureAlgorithm::GroundtoneHypothesisBuilder gtSeed( testSpec );
   const RealVector& vec = gtSeed.execute( 20 );
   msg << Msg::Info << vec << Msg::EndReq;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devPeakFinder2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::devPeakFinder2()
{
   Logger msg( "devPeakFinder2" );
   msg << Msg::Info << "Running devPeakFinder2..." << Msg::EndReq;

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

void DevSuite::devMlp2()
{
   Logger msg( "devMlp2" );
   msg << Msg::Info << "In devMlp2..." << Msg::EndReq;

   std::vector< RealVector > inputData, outputData;
   inputData.push_back( realVector( 1, 1 ) );
   outputData.push_back( realVector( 1 ) );
   inputData.push_back( realVector( 1, 0 ) );
   outputData.push_back( realVector( 0 ) );
   inputData.push_back( realVector( 0, 1 ) );
   outputData.push_back( realVector( 0 ) );
   inputData.push_back( realVector( 0, 0 ) );
   outputData.push_back( realVector( 0 ) );

   Mva::MultilayerPerceptron net( 2, 1 );
   net.addHiddenLayer( 4 );
   net.addHiddenLayer( 3 );
   net.addHiddenLayer( 2 );
   net.addHiddenLayer( 2 );
   net.build();

   for ( size_t i = 0; i < inputData.size(); ++i )
   {
      msg << Msg::Info << "Input = " << inputData[ i ] << ", eval = " << net.evaluate( inputData[ i ] ) << ", expected = " << outputData[ i ] << Msg::EndReq;
   }

   net.train( inputData, outputData );

   for ( size_t i = 0; i < inputData.size(); ++i )
   {
      msg << Msg::Info << "Input = " << inputData[ i ] << ", eval = " << net.evaluate( inputData[ i ] ) << ", expected = " << outputData[ i ] << Msg::EndReq;
   }
}

void DevSuite::devMlp()
{
   Logger msg( "devMlp" );
   msg << Msg::Info << "In devMlp..." << Msg::EndReq;

   TRandom3 rand( 1 );

   std::vector< RealVector > inputData;
   std::vector< RealVector > outputData;
   RealVector xUp, yUp, xDown, yDown;

   size_t nTrainSamples = 1000;
   for ( size_t iTrainSample = 0; iTrainSample < nTrainSamples; ++iTrainSample )
   {
      double x = rand.Uniform( -1, 1 );
      double y = rand.Uniform( -1, 1 );
      double z = -1;
      if ( x * y > 0 )
      {
         z = 1;
         xUp.push_back( x );
         yUp.push_back( y );
      }
      else
      {
         xDown.push_back( x );
         yDown.push_back( y );
      }
      RealVector v;
      v.push_back( x );
      v.push_back( y );
      inputData.push_back( v );
      outputData.push_back( RealVector( 1, z ) );
   }

   gPlotFactory().createPlot( "devMlp/trainingSet" );
   gPlotFactory().createScatter( xUp, yUp, Plotting::MarkerDrawAttr( Qt::red ) );
   gPlotFactory().createScatter( xDown, yDown, Plotting::MarkerDrawAttr( Qt::blue ) );

   Mva::MultilayerPerceptron network( 2, 1 );
   network.addHiddenLayer( 4 );
   network.addHiddenLayer( 2 );
   network.build();
   // Mva::RootMlp network( "i0,i1:4:2:o0" );

   network.train( inputData, outputData );

   RealVector test( 2, 1 );
   msg << Msg::Info << "Dimension check: " << network.evaluate( test ) << Msg::EndReq;

   Math::RealFunctionPtr func( &tanh );
   const RealVector& xEval = Utils::createRangeReal( -10, 10, 100 );
   const RealVector& yEval = func.evalMany( xEval );

   gPlotFactory().createPlot( "devMlp/tanh" );
   gPlotFactory().createGraph( xEval, yEval, Qt::blue );

   RealVector xUpPredicted, yUpPredicted, xDownPredicted, yDownPredicted;
   for ( size_t i = 0; i < inputData.size(); ++i )
   {
      RealVector output = network.evaluate( inputData[ i ] );
      // msg << Msg::Info << "output = " << output << ", desired = " << outputData[ i ] << Msg::EndReq;
      if ( output [ 0 ] > 0 )
      {
         xUpPredicted.push_back( inputData[ i ][ 0 ] );
         yUpPredicted.push_back( inputData[ i ][ 1 ] );
      }
      else
      {
         xDownPredicted.push_back( inputData[ i ][ 0 ] );
         yDownPredicted.push_back( inputData[ i ][ 1 ] );
      }
   }

   gPlotFactory().createPlot( "devMlp/testSet" );
   gPlotFactory().createScatter( xUpPredicted, yUpPredicted, Plotting::MarkerDrawAttr( Qt::red ) );
   gPlotFactory().createScatter( xDownPredicted, yDownPredicted, Plotting::MarkerDrawAttr( Qt::blue ) );

   return;
}

