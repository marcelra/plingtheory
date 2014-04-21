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

   devHistogram();
   // devRebinSRSpec();
   // devFourierTemplates();
   // devSamples();

   // devPeakFinder2();
   // devMlp();
   devMcmc();
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
/// testPdf
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::testPdf()
{
   Logger msg( "testPdf" );
   msg << Msg::Info << "Running testPdf..." << Msg::EndReq;

   size_t nPoints = 5000;

   const RealVector xEval = Utils::createRangeReal( -10, 10, nPoints );

   Math::GaussPdf gauss( 0, 1 );

   Math::IRealFunction::Ptr func( new Math::RealMemFunction< Math::GaussPdf >( &Math::GaussPdf::getDensity, &gauss ) );
   const RealVector& gaussEval = func->evalMany( xEval );

   gPlotFactory().createPlot( "testPdf/GaussPdf" );
   gPlotFactory().createGraph( xEval, gaussEval );

   double integralApprox = 0;
   double deltaX = 20.0 / nPoints;
   for ( size_t i = 0; i < gaussEval.size(); ++i )
   {
      integralApprox += gaussEval[ i ] * deltaX;
   }
   msg << Msg::Info << "Integral = " << integralApprox << Msg::EndReq;

   size_t nGauss = 2000;
   RealVector sampling( nGauss );
   for ( size_t i = 0; i < nGauss; ++i )
   {
      sampling[ i ] = gRandom->Uniform( -100, 100 );
   }

   const RealVector& xEvalKern = Utils::createRangeReal( -200, 200, nPoints );

   Math::UniformPdf uniform( -100, 100 );
   func.reset( new Math::RealMemFunction< Math::UniformPdf >( &Math::UniformPdf::getDensity, &uniform ) );
   const RealVector& uniformEval = func->evalMany( xEvalKern );

   Math::KernelPdf kern( Math::IPdf::CPtr( new Math::GaussPdf( 0, 25 ) ), sampling );
   func.reset( new Math::RealMemFunction< Math::KernelPdf >( &Math::KernelPdf::getDensity, &kern ) );
   const RealVector& kernEval = func->evalMany( xEvalKern );

   gPlotFactory().createPlot( "testPdf/KernelPdf" );
   gPlotFactory().createGraph( xEvalKern, kernEval );
   gPlotFactory().createGraph( xEvalKern, uniformEval, Qt::red );
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devSamples
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::devSamples()
{
   size_t numSamples = 10 * 44100;

   SamplingInfo samplingInfo( 44100 );
   RawPcmData pcmData( samplingInfo, numSamples, 0 );

   double frequency = 110;
   double phase = 0;
   double amp = 0.5;
   double phaseStep = samplingInfo.getPhaseStepPerSample( frequency );

   size_t i = 0;
   while ( i < numSamples )
   {
      size_t tStart = samplingInfo.convertSecsToSamples( 0.1 ) + i;
      size_t tEnd = samplingInfo.convertSecsToSamples( 0.50 ) + i;
      size_t tRestart = samplingInfo.convertSecsToSamples( 1.0 ) + i;
      double phaseAdd = M_PI / ( tEnd - tStart );

      for ( ; i < tStart && i < numSamples ; ++i )
      {
         pcmData[ i ] = amp * sin( phase );
         phase += phaseStep;
      }

      phaseStep += phaseAdd;
      for ( ; i < tEnd && i < numSamples; ++i )
      {
         pcmData[ i ] = amp * sin( phase );
         phase += phaseStep;
      }

      phaseStep -= phaseAdd;
      for ( ; i < tRestart && i < numSamples; ++i )
      {
         pcmData[ i ] = amp * sin( phase );
         phase += phaseStep;
      }

      ++i;
   }

   TGraph* grData = RootUtilities::createGraph( pcmData );
   grData->Draw( "AL" );

   WaveAnalysis::SpectralReassignmentTransform transform( samplingInfo, 1024, 3*1024, 4 );
   WaveAnalysis::StftData::Ptr stft = transform.execute( pcmData );

   Visualisation::RebinnedSRGraph graph( *stft );
   graph.create();

   MultiChannelRawPcmData mc( new RawPcmData( pcmData ) );
   WaveFile::write( "sinefrag2.wav", mc );
}

void DevSuite::devHistogram()
{
   TRandom3 rand( 0 );

   size_t nBins = 100;
   double xMin = -5;
   double xMax = 5;

   Math::RegularAccumArray hist( nBins, xMin, xMax );
   size_t nDraws = 500;
   double w = ( xMax - xMin ) / nDraws;

   for ( size_t i = 0; i < nDraws; ++i )
   {
      double x = rand.Gaus( 0, 1 );
      hist.add( x, w );
   }

   Math::GaussPdf pdf( 0, 1 );
   const RealVector& xEval = Utils::createRangeReal( xMin, xMax, 100 );
   Math::RealMemFunction<Math::GaussPdf> pdfFunc( &Math::GaussPdf::getDensity, &pdf );
   const RealVector& yEval = pdfFunc.evalMany( xEval );

   gPlotFactory().createPlot( "testHistogram/Gauss sampling" );
   gPlotFactory().createHistogram( hist );
   gPlotFactory().createGraph( xEval, yEval, Qt::blue );
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

   TRandom3 rand( 0 );

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
   gPlotFactory().createScatter( xUp, yUp, Qt::red );
   gPlotFactory().createScatter( xDown, yDown, Qt::blue );

   Mva::MultilayerPerceptron network( 2, 1 );
   network.addHiddenLayer( 8 );
   network.addHiddenLayer( 4 );
   network.addHiddenLayer( 3 );
   network.addHiddenLayer( 2 );
   network.addHiddenLayer( 1 );
   network.build();

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
   gPlotFactory().createScatter( xUpPredicted, yUpPredicted, Qt::red );
   gPlotFactory().createScatter( xDownPredicted, yDownPredicted, Qt::blue );

   return;
}

class MultivariateGaussObjective : public Math::IObjectiveFunction
{
   public:
      MultivariateGaussObjective( const RealVector& muVec, const RealVector& sigmaVec ) :
         m_muVec( muVec ),
         m_sigmaVec( sigmaVec )
      {
         assert( m_muVec.size() == m_sigmaVec.size() );
         for ( size_t i = 0; i < m_muVec.size(); ++i )
         {
            m_pdfs.push_back( Math::GaussPdf( m_muVec[ i ], m_sigmaVec[ i ] ) );
         }
      }

      size_t getNumParameters() const
      {
         return m_muVec.size();
      }

      double evaluate( const RealVector& x ) const
      {
         assert( x.size() == getNumParameters() );
         double p = 1;
         for ( size_t i = 0; i < x.size(); ++i )
         {
            p *= m_pdfs[ i ].getDensity( x[ i ] );
         }
         return p;
      }

   private:
      RealVector                    m_muVec;
      RealVector                    m_sigmaVec;
      std::vector< Math::GaussPdf > m_pdfs;

};

void DevSuite::devMcmc()
{
   Logger msg( "devMcmc" );
   msg << Msg::Info << "Running devMcmc..." << Msg::EndReq;

   size_t numSamples = 1;

   MultivariateGaussObjective objFunc( realVector( 1, 0 ) , realVector( 20, 10 ) );

   Math::McmcOptimiser mcmc( objFunc );
   mcmc.setStartValues( Math::RealVectorEnsemble( numSamples, realVector( 10, 10 ) ) );
   mcmc.setStepSize( 0.1 );
   mcmc.setNumIterations( 10000 );
   mcmc.setBurninSkip( 0 );
   const Math::RealVectorEnsemble& solution = mcmc.solve();

   double yEval = 0;
   double yWidth = 0.1;

   RealVector xData( solution.size() );
   RealVector yData( solution.size() );

   for ( size_t i = 0; i < solution.size(); ++i )
   {
      xData[ i ] = solution[ i ][ 0 ];
      yData[ i ] = solution[ i ][ 1 ];
   }

   gPlotFactory().createPlot( "devMcmc/twoDimScatter" );
   gPlotFactory().createScatter( xData, yData, Qt::black );


   const RealVector& xArr = Utils::createRangeReal( -5, 5, 50 );
   RealVector objFuncEval( xArr.size() );

   for ( size_t i = 0; i < xArr.size(); ++i )
   {
      objFuncEval[ i ] = objFunc.evaluate( realVector( xArr[ i ], yEval ) );
   }

   gPlotFactory().createPlot( "devMcmc/distribution" );
   // gPlotFactory().createGraph( xArr, accArr.getAllBinContents() );
   gPlotFactory().createGraph( xArr, objFuncEval, Qt::blue );

}

