#include "TestMath.h"

#include "IPlotFactory.h"
#include "Logger.h"
#include "RootUtilities.h"
#include "TestDataSupply.h"

#include "ComposedRealFuncWithDerivative.h"
#include "GaussPdf.h"
#include "GradDescOptimiser.h"
#include "NewtonSolver1D.h"
#include "RealMemFunction.h"
#include "SampledMovingAverage.h"
#include "TwoDimExampleObjective.h"
#include "McmcOptimiser.h"

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
   testMcmc();
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
   const RealVector& movAvg = movAvgCalc.calculate( dataSet );

   gPlotFactory().createPlot( "testSampledMovingAverage/graph" );
   gPlotFactory().createGraph( dataSet );
   gPlotFactory().createGraph( movAvg, Qt::blue );

   for ( size_t i = 0; i < movAvg.size(); ++i )
   {
      msg << Msg::Verbose << "Moving average of sample " << i << ": " << movAvg[i] << Msg::EndReq;
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

   /// Fill initial TwoTuple.
   size_t nElements = 2;
   TwoTuple filledAfterConstruction( nElements );
   filledAfterConstruction.getX()[ 0 ] = 0;
   filledAfterConstruction.getX()[ 1 ] = 1;
   filledAfterConstruction.getY()[ 0 ] = 2;
   filledAfterConstruction.getY()[ 1 ] = 3;

   /// Create TwoTuple from vectors
   TwoTuple filledAtConstruction( filledAfterConstruction.getX(), filledAfterConstruction.getY() );

   /// Create TwoTuple by adding rows.
   TwoTuple fillHorizontal;
   for ( size_t i = 0; i < filledAfterConstruction.getNumElements(); ++i )
   {
      fillHorizontal.addTuple( filledAfterConstruction.getTuple( i ) );
   }

   /// Create TwoTuple by using the add(x,y) method.
   TwoTuple fillVertical;
   for ( size_t i = 0; i < filledAfterConstruction.getNumElements(); ++i )
   {
      fillVertical.add( filledAfterConstruction.getX()[ i ], filledAfterConstruction.getY()[ i ] );
   }

   /// Create TwoTuple with assignment operator.
   TwoTuple assignmentOp( 100 );
   assignmentOp = filledAfterConstruction;

   /// Check that the previous number of elements (100) is now overwritten.
   if ( assignmentOp.getNumElements() != filledAfterConstruction.getNumElements() )
   {
      throw ExceptionTestFailed( "testTwoTuple", "Assignment operator does not overwrite number of elements." );
   }

   /// Test copy constructor.
   TwoTuple copyConstructor( filledAfterConstruction );

   /// Test all rows.
   for ( size_t i = 0; i < filledAfterConstruction.getNumElements(); ++i )
   {
      const RealVector& refTuple = filledAfterConstruction.getTuple( i );
      RealVector testTuple;
      testTuple = filledAtConstruction.getTuple( i );
      if ( !isEqual( testTuple, refTuple ) )
      {
         throw ExceptionTestFailed( "testTwoTuple", "Unexpected differences between test and ref for filledAtConstruction." );
      }
      testTuple = fillHorizontal.getTuple( i );
      if ( !isEqual( testTuple, refTuple ) )
      {
         throw ExceptionTestFailed( "testTwoTuple", "Unexpected differences between test and ref for fillHorizontal." );
      }
      testTuple = fillVertical.getTuple( i );
      if ( !isEqual( testTuple, refTuple ) )
      {
         throw ExceptionTestFailed( "testTwoTuple", "Unexpected differences between test and ref for fillVertical." );
      }
      testTuple = assignmentOp.getTuple( i );
      if ( !isEqual( testTuple, refTuple ) )
      {
         throw ExceptionTestFailed( "testTwoTuple", "Unexpected differences between test and ref for assignmentOp." );
      }
      testTuple = copyConstructor.getTuple( i );
      if ( !isEqual( testTuple, refTuple ) )
      {
         throw ExceptionTestFailed( "testTwoTuple", "Unexpected differences between test and ref for copyConstructor." );
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

   /// Construct a acummulation array object with 2 bins centered around 0 and 1.
   const size_t nBins = 2;
   const double minX = -0.5;
   const double maxX = 1.5;

   Math::RegularAccumArray hist( nBins, minX, maxX );

   /// Test finding of minimum and maximum.
   if ( minX != hist.getMinX() || maxX != hist.getMaxX() )
   {
      throw ExceptionTestFailed( "testRegularAccumArray", "Minimum or maximum are not found correctly." );
   }

   /// Test underflow, overflow.
   hist.add( minX - 0.1, 1 );
   hist.add( maxX + 0.1, 1 );

   if ( hist.getUnderflow() != 1 )
   {
      throw ExceptionTestFailed( "testRegularAccumArray", "Underflow not correct." );
   }
   if ( hist.getOverflow() != 1 )
   {
      throw ExceptionTestFailed( "testRegularAccumArray", "Overflow not correct." );
   }
   msg << Msg::Info << "Overflow and underflow produced correct results." << Msg::EndReq;

   /// Fill accumulation arry with 100 test numbers.
   const size_t nFills = 100;
   double x = hist.getMinX();
   const double step = ( hist.getMaxX() - hist.getMinX() ) / nFills;
   for ( size_t i = 0; i < nFills; ++i )
   {
      hist.add( x, 1 );
      x += step;
   }

   if ( hist.getAllBinContents()[ 0 ] != ( nFills / 2 ) )
   {
      throw ExceptionTestFailed( "testRegularAccumArray", "Histogram not filled correctly." );
   }
   if ( hist.getAllBinContents()[ 1 ] != ( nFills / 2 ) )
   {
      throw ExceptionTestFailed( "testRegularAccumArray", "Histogram not filled correctly." );
   }

   msg << Msg::Info << "Filling of histogram is correct." << Msg::EndReq;

   /// Fill with the same numbers but with weight -1.
   x = hist.getMinX();
   for ( size_t i = 0; i < nFills; ++i )
   {
      hist.add( x, -1 );
      x += step;
   }

   /// Check that bin contents are now 0.
   const IAccumArray::Bin& bin0 = hist.getBin( 0 );
   const IAccumArray::Bin& bin1 = hist.getBin( 1 );

   if ( bin0.getContents() != 0 )
   {
      throw ExceptionTestFailed( "testRegularAccumArray", "Histogram bin contents are expected to be zero for bin 0." );
   }
   if ( bin1.getContents() != 0 )
   {
      throw ExceptionTestFailed( "testRegularAccumArray", "Histogram bin contents are expected to be zero for bin 1." );
   }

   /// Test entries.
   const TwoTuple& entries0 = bin0.getEntries();

   /// The number of entries should be even since it is filled twice with the same number of fills.
   assert( entries0.getNumElements() % 2 == 0 );

   /// Check that each entry has an opposity entry at the same x with opposite weight.
   for ( size_t i = 0; i < entries0.getNumElements() / 2 - 1; ++i )
   {
      double xVal = entries0.getX()[ i ];
      double wVal = entries0.getY()[ i ];
      size_t foundIndex = entries0.getNumElements();
      for ( size_t j = i + 1; j < entries0.getNumElements(); ++j )
      {
         if ( xVal == entries0.getX()[ j ] )
         {
            foundIndex = j;
            if ( wVal != -entries0.getY()[ j ] )
            {
               throw ExceptionTestFailed( "testRegularAccumArray", "Weights of histogram entries are not correct." );
            }
            break;
         }
      }
      if ( foundIndex == entries0.getNumElements() )
      {
         throw ExceptionTestFailed( "testRegularAccumArray", "Histogram entries are not correct." );
      }
   }
   msg << Msg::Info << "Histogram entries are tested to be working correctly." << Msg::EndReq;

   /// Test done.
   msg << Msg::Info << "Test done." << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testNewtonSolver1D
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestMath::testNewtonSolver1D()
{
   Logger msg( "testNewtonSolver1D" );
   msg << Msg::Info << "Running testNewtonSolver1D..." << Msg::EndReq;

   size_t nPoints = 5000;
   double min = -10;
   double max = 10;
   const RealVector& xEval = Utils::createRangeReal( min, max, nPoints );

   Math::GaussPdf gaussPdf( 0, 1 );
   Math::RealMemFunction< Math::GaussPdf > integral( &Math::GaussPdf::getIntegral, &gaussPdf );
   Math::RealMemFunction< Math::GaussPdf > density( &Math::GaussPdf::getDensity, &gaussPdf );
   Math::ComposedRealFuncWithDerivative pdfAsFunc( integral, density );

   const RealVector& yIntegral = pdfAsFunc.evalMany( xEval );
   const RealVector& yDensity = pdfAsFunc.evalDerivMany( xEval );

   gPlotFactory().createPlot( "RealFunction/GaussPdf" );
   gPlotFactory().createGraph( xEval, yIntegral );
   gPlotFactory().createGraph( xEval, yDensity );

   Math::NewtonSolver1D solver;
   Math::NewtonSolver1D::Result result = solver.solve( pdfAsFunc, 0.3, 2 );
   msg << Msg::Info << "NewtonSolver1D foound result: " << result.getSolution() << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MultivariateGaussObjective (needed by testMcmc).
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testMcmc
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestMath::testMcmc()
{
   Logger msg( "testMcmc" );
   msg << Msg::Info << "Running testMcmc..." << Msg::EndReq;

   size_t numSamples = 1;

   MultivariateGaussObjective objFunc( realVector( 50, 0 ) , realVector( 20, 10 ) );

   Math::McmcOptimiser mcmc( objFunc );
   mcmc.setStartValues( Math::RealVectorEnsemble( numSamples, realVector( 10, 10 ) ) );
   mcmc.setStepSize( 1 );
   mcmc.setNumIterations( 50000 );
   mcmc.setBurninSkip( 0 );
   const Math::RealVectorEnsemble& solution = mcmc.solve();

   RealVector xData( solution.size() );
   RealVector yData( solution.size() );

   for ( size_t i = 0; i < solution.size(); ++i )
   {
      xData[ i ] = solution[ i ][ 0 ];
      yData[ i ] = solution[ i ][ 1 ];
   }

   gPlotFactory().createPlot( "testMcmc/twoDimScatter" );
   gPlotFactory().createScatter( xData, yData, Plotting::MarkerDrawAttr( Qt::black, Plotting::MarkerPlus, 3, 2, false ) );

   double yEval = 0;
   double yWidth = 0.5;
   size_t nBinsEval = 100;
   double xMin = -50;
   double xMax = 200;

   const RealVector& xArr = Utils::createRangeReal( xMin, xMax, nBinsEval );
   RealVector objFuncEval( xArr.size() );

   for ( size_t i = 0; i < xArr.size(); ++i )
   {
      objFuncEval[ i ] = objFunc.evaluate( realVector( xArr[ i ], yEval ) ) * yWidth;
   }

   double yMin = yEval - 0.5 * yWidth;
   double yMax = yEval + 0.5 * yWidth;

   Math::RegularAccumArray regAccArr( nBinsEval, xMin, xMax );

   double weight = 1. / xData.size() / regAccArr.getBinWidth();

   for ( size_t i = 0; i < xData.size(); ++i )
   {
      if ( yData[ i ] > yMin && yData[ i ] < yMax )
      {
         regAccArr.add( xData[ i ], weight );
      }
   }


   gPlotFactory().createPlot( "testMcmc/distribution" );
   gPlotFactory().createGraph( xArr, objFuncEval, Qt::blue );
   gPlotFactory().createHistogram( regAccArr );

}

