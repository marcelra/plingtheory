#include "TestMath.h"

#include "Logger.h"

#include "GradDescOptimiser.h"
#include "TwoDimExampleObjective.h"

using namespace Math;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testMath
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestMath::testMath()
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

}


