#include "TestMath.h"

#include "Logger.h"
#include "RootUtilities.h"
#include "TestDataSupply.h"

#include "GradDescOptimiser.h"
#include "SampledMovingAverage.h"
#include "TwoDimExampleObjective.h"

#include "TGraph.h"

#include <cmath>

using namespace Math;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testMath
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestMath::execute()
{
   testTwoDimExampleObjective();
   testGradDescOptimiser();
   testSampledMovingAverage();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testTwoDimExampleObjective
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestMath::testTwoDimExampleObjective()
{
   Logger msg( "testTwoDimExampleObjective" );
   msg << Msg::Info << "In testTwoDimExampleObjective..." << Msg::EndReq;
   TwoDimExampleObjective obj;
   RealVector x( 2, 0 );
   msg << Msg::Info << "The objective function value at x " << x << " = " << obj.evaluate( x ) << Msg::EndReq;
   msg << Msg::Info << "The gradient at x = " << x << " = " << obj.calculateGradient( x ) << Msg::EndReq;
   x[0] = 2;
   x[1] = 1;
   msg << Msg::Info << "The objective function value at x " << x << " = " << obj.evaluate( x ) << Msg::EndReq;
   msg << Msg::Info << "The gradient at x = " << x << " = " << obj.calculateGradient( x ) << Msg::EndReq;
   msg << Msg::Info << "Test done" << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testGradDescOptimiser
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestMath::testGradDescOptimiser()
{
   Logger msg( "testGradDescOptimiser" );
   msg << Msg::Info << "In testGradDescOptimiser..." << Msg::EndReq;

   TwoDimExampleObjective obj;

   RealVector startVal( 2, 0 );
   startVal[0] = 50;
   startVal[1] = 100;

   GradDescOptimiser opt( obj, startVal );
   const RealVector& solution = opt.solve();

   msg << Msg::Info << "Found solution: " << solution << Msg::EndReq;
   msg << Msg::Info << "Test done" << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testSampledMovingAverage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestMath::testSampledMovingAverage()
{
   Logger msg( "testSampledMovingAverage" );
   msg << Msg::Info << "In testSampledMovingAverage..." << Msg::EndReq;

   const RealVector& dataSet = TestDataSupply::createNoiseAndPeaks();

   std::vector< double > movAvgWeights;
   double lambda = 20;
   for ( int i = -20; i <= 20; ++i )
   {
      movAvgWeights.push_back( 1 / lambda * exp( - i*i / lambda ) );
   }
   Math::SampledMovingAverage movAvgCalc( Math::SampledMovingAverage::createGaussianFilter( 41, 20 ) );
   RealVector movAvg = movAvgCalc.calculate( dataSet );

   TGraph* grOriginal = RootUtilities::createGraph( dataSet );
   TGraph* grMovAvg   = RootUtilities::createGraph( movAvg );

   grOriginal->Draw( "AL" );
   grMovAvg->Draw( "LSAME" );
   grMovAvg->SetLineColor( kBlue );

   for ( size_t i = 0; i < movAvg.size(); ++i )
   {
      msg << Msg::Info << movAvg[i] << Msg::EndReq;
   }
}


