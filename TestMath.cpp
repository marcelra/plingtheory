#include "TestMath.h"

/// Helper classes.
#include "IPlotFactory.h"
#include "Logger.h"
#include "RootUtilities.h"
#include "TestDataSupply.h"

/// Algorithms being tested.
#include "Chi2FitObjective.h"
#include "ComposedRealFuncWithDerivative.h"
#include "GaussPdf.h"
#include "GradDescOptimiser.h"
#include "Hypercube.h"
#include "KernelPdf.h"
#include "LinearInterpolator.h"
#include "LineSearchGradDescOptimiser.h"
#include "McmcOptimiser.h"
#include "NewtonSolver1D.h"
#include "ParticleSwarmOptimiser.h"
#include "PolynomialFitFunction.h"
#include "RealMemFunction.h"
#include "SampledMovingAverage.h"
#include "TwoDimExampleObjective.h"
#include "UniformPdf.h"


#include <cmath>

using namespace Math;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testMath
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestMath::execute()
{
   Logger msg( "TestMath::execute" );
   msg << Msg::Info << "Executing math tests..." << Msg::EndReq;

   /// Gradient-based optimisation algorithms.
   testGradDescOptimiser();
   testTwoDimExampleObjective();

   /// Fitting.
   testSimpleFit();

   /// Stochastic optimisation algorithms.
   testParticleSwarm();
   testMcmc();
   testSampledMovingAverage();

   /// Math containers.
   testTwoTuple();
   testRegularAccumArray();

   /// Other/uncategorized.
   testNewtonSolver1D();
   testPdf();
   testLinearInterpolator();

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
/// testSimpleFit
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestMath::testSimpleFit()
{
   Logger msg( "testSimpleFit" );
   msg << Msg::Info << "Running testSimpleFit..." << Msg::EndReq;


   /// Define solution and startValues.
   const RealVector& solution = realVector( 0, 1, -0.5, 1./6., -1/24. );
   const RealVector& startValues = realVector( 0, 0, 0, 0, 0 );

   Math::FitFunctionBase* fitFunction = new Math::PolynomialFitFunction( solution.size() - 1 );
   fitFunction->setParameters( solution );

   /// Create f(x) = x - 1/2 * x^2 + 1/6 * x^3 with added noise sampled from Uniform(-1,1).
   RandomNumberGenerator rng( 1 );
   size_t numSamples = 100;

   RealVector xData( numSamples );
   RealVector yData( numSamples );
   RealVector ySigma2( numSamples, 1 );
   for ( size_t i = 0; i < numSamples; ++i )
   {
      double x = ( i - 0.5 * numSamples ) / 10.0;
      xData[ i ] = x;
      yData[ i ] = (*fitFunction)( x ) + 1 * rng.uniform( -1, 1 );
   }

   /// Define the fit objective.
   Math::Chi2FitObjective fitObj( xData, yData, ySigma2, fitFunction );

   Math::LineSearchGradDescOptimiser lineSearchOptimiser( fitObj );
   lineSearchOptimiser.setMaxIterations( 100000 );
   lineSearchOptimiser.setMinGradLength( 1 );
   lineSearchOptimiser.setMinObjFuncValChange( 1e-8 );
   lineSearchOptimiser.setLoggerThreshold( Msg::Info );
   const RealVector& lsOptSolution = lineSearchOptimiser.solve( startValues );
   msg << Msg::Info << "Solution found by LineSearchGradDescOptimiser = " << lsOptSolution << Msg::EndReq;

   /// Draw the data set.
   gPlotFactory().createPlot( "testSimpleFit/FitResult" );
   gPlotFactory().createScatter( xData, yData );

   /// Draw the real solution.
   fitFunction->setParameters( solution );
   gPlotFactory().createGraph( xData, fitFunction->evalMany( xData ), Qt::red );

   /// Draw the solution fitted by the line-search optimiser.
   fitFunction->setParameters( lsOptSolution );
   gPlotFactory().createGraph( xData, fitFunction->evalMany( xData ), Qt::green );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testSampledMovingAverage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestMath::testSampledMovingAverage()
{
   Logger msg( "testSampledMovingAverage" );
   msg << Msg::Info << "In testSampledMovingAverage..." << Msg::EndReq;

   Math::SampledMovingAverage movAvgCalc( Math::SampledMovingAverage::createGaussianFilter( 41, 20 ) );

   RealVector constDataSet( 100, 1 );

   gPlotFactory().createPlot( "testSampledMovingAverage/const" );
   gPlotFactory().createGraph( constDataSet );
   gPlotFactory().createGraph( movAvgCalc.calculate( constDataSet ), Qt::blue );

   RealVector linearDataSet = Utils::createRangeReal( 4, 1, 100 );
   gPlotFactory().createPlot( "testSampledMovingAverage/linear" );
   gPlotFactory().createGraph( linearDataSet );
   gPlotFactory().createGraph( movAvgCalc.calculate( linearDataSet ), Qt::blue );

   RealVector linearPerturbDataSet = Utils::createRangeReal( 4, 1, 101 );
   linearPerturbDataSet[ 50 ] = 10;
   gPlotFactory().createPlot( "testSampledMovingAverage/linearPerturb" );
   gPlotFactory().createGraph( linearPerturbDataSet );
   gPlotFactory().createGraph( movAvgCalc.calculate( linearPerturbDataSet ), Qt::blue );

   const RealVector& dataSet = TestDataSupply::createNoiseAndPeaks();

   std::vector< double > movAvgWeights;
   double lambda = 20;
   for ( int i = -20; i <= 20; ++i )
   {
      movAvgWeights.push_back( 1 / lambda * exp( - i*i / lambda ) );
   }
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
/// testParticleSwarm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestMath::testParticleSwarm()
{
   Logger msg( "testParticleSwarm" );
   msg << Msg::Info << "Running testParticleSwarm..." << Msg::EndReq;

   RealVector minVec( 2, -10 );
   RealVector maxVec( 2, 10 );
   Math::Hypercube solutionSpace( minVec, maxVec );

   Math::TwoDimExampleObjective objFunc;

   Math::ParticleSwarmOptimiser pso( objFunc, 400, solutionSpace );

   std::vector< std::vector< RealVector > > swarmTracker;
   const RealVector& result = pso.solve( 100, &swarmTracker );

   Plotting::Palette pal = Plotting::Palette::heatPalette();

   gPlotFactory().createPlot( "testParticleSwarm/swarmTracking" );
   for ( size_t i = 0; i < swarmTracker.size(); ++i )
   {
      RealVector xData( swarmTracker[ i ].size() );
      RealVector yData( swarmTracker[ i ].size() );

      for ( size_t j = 0; j < xData.size(); ++j )
      {
         xData[ j ] = swarmTracker[ i ][ j ][ 0 ];
         yData[ j ] = swarmTracker[ i ][ j ][ 1 ];
      }

      QColor colour = pal.getColour( static_cast< double >( i ) / swarmTracker.size() );
      gPlotFactory().createScatter( xData, yData, Plotting::MarkerDrawAttr( colour, Plotting::MarkerPlus, 3 ) );
   }

   msg << Msg::Info << "Result = " << result << Msg::EndReq;

}

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testPdf
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestMath::testPdf()
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
/// devLinearInterpolator
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestMath::testLinearInterpolator()
{
   Logger msg( "testLinearInterpolator" );
   msg << Msg::Info << "Running testLinearInterpolator..." << Msg::EndReq;

   RealVector x = realVector( 0, 1, 2, 4, 5, 6, 8 );
   RealVector y = realVector( 0, 1, 2, 4, 5, 6, 8 );

   Math::LinearInterpolator interpolateY( x, y );

   const RealVector xEval = Utils::createRangeReal( 0, 8, 10 );

   for ( size_t i = 0; i < xEval.size(); ++i )
   {
      double diff = xEval[ i ] - interpolateY( xEval[ i ] );
      msg << Msg::Info << "x = " << xEval[ i ] << ", f(x) = x => x - f(x) = " << diff << " = 0" << Msg::EndReq;
   }

   return;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// testDecisionTree
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestMath::testDecisionTree()
{
   Logger msg( "testDecisionTree" );

   msg << Msg::Info << "Running testDecisionTree..." << Msg::EndReq;




   return;
}


