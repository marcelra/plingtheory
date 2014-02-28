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
   Logger msg( "TestMath::execute" );
   msg << Msg::Info << "Executing Math namespace tests..." << Msg::EndReq;
   testTwoDimExampleObjective();
   testGradDescOptimiser();
   testSampledMovingAverage();
   testTwoTuple();
   testRegularAccumArray();
   msg << Msg::Info << "Math tests done." << Msg::EndReq;
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
   msg << Msg::Info << "Test done." << Msg::EndReq;
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
   msg << Msg::Info << "Test done." << Msg::EndReq;
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
      msg << Msg::Verbose << movAvg[i] << Msg::EndReq;
   }

   msg << Msg::Info << "Test done." << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testTwoTuple
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestMath::testTwoTuple()
{
   Logger msg( "testTwoTuple" );
   msg << Msg::Info << "In testTwoTuple..." << Msg::EndReq;

   size_t nElements = 2;
   TwoTuple filledAfterConstruction( nElements );
   filledAfterConstruction.getX()[ 0 ] = 0;
   filledAfterConstruction.getX()[ 1 ] = 1;
   filledAfterConstruction.getY()[ 0 ] = 2;
   filledAfterConstruction.getY()[ 1 ] = 3;

   TwoTuple filledAtConstruction( filledAfterConstruction.getX(), filledAfterConstruction.getY() );

   TwoTuple fillHorizontal( 0 );
   for ( size_t i = 0; i < filledAfterConstruction.getNumElements(); ++i )
   {
      fillHorizontal.addTuple( filledAfterConstruction.getTuple( i ) );
   }

   TwoTuple fillVertical;
   for ( size_t i = 0; i < filledAfterConstruction.getNumElements(); ++i )
   {
      fillVertical.add( filledAfterConstruction.getX()[ i ], filledAfterConstruction.getY()[ i ] );
   }

   TwoTuple assignmentOp( 100 );
   assignmentOp = filledAfterConstruction;

   TwoTuple copyConstructor( filledAfterConstruction );

   for ( size_t i = 0; i < filledAfterConstruction.getNumElements(); ++i )
   {
      const RealVector& refTuple = filledAfterConstruction.getTuple( i );
      RealVector testTuple;
      testTuple = filledAtConstruction.getTuple( i );
      if ( !isEqual( testTuple, refTuple ) )
      {
         throw ExceptionTestFailed( "testTwoTuple", "Unexpected differences between test and ref" );
      }
      testTuple = fillHorizontal.getTuple( i );
      if ( !isEqual( testTuple, refTuple ) )
      {
         throw ExceptionTestFailed( "testTwoTuple", "Unexpected differences between test and ref" );
      }
      testTuple = fillVertical.getTuple( i );
      if ( !isEqual( testTuple, refTuple ) )
      {
         throw ExceptionTestFailed( "testTwoTuple", "Unexpected differences between test and ref" );
      }
      testTuple = assignmentOp.getTuple( i );
      if ( !isEqual( testTuple, refTuple ) )
      {
         throw ExceptionTestFailed( "testTwoTuple", "Unexpected differences between test and ref" );
      }
      testTuple = copyConstructor.getTuple( i );
      if ( !isEqual( testTuple, refTuple ) )
      {
         throw ExceptionTestFailed( "testTwoTuple", "Unexpected differences between test and ref" );
      }
   }

   msg << Msg::Info << "Test done." << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testRegularAccumArray
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestMath::testRegularAccumArray()
{
   Logger msg( "testRegularAccumArray" );
   msg << Msg::Info << "In testRegularAccumArray..." << Msg::EndReq;

   msg << Msg::Info << "Test done." << Msg::EndReq;
}


