#include "LineSearchGradDescOptimiser.h"

#include "Logger.h"

#include <limits>

namespace Math
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LineSearchGradDescOptimiser::LineSearchGradDescOptimiser( const IObjectiveFunction& objFunc, const std::string& algorithmName, const AlgorithmBase* parent ) :
   AlgorithmBase( algorithmName, parent ),
   m_objFunc( objFunc ),
   m_maxIterations( 100 ),
   m_minGradModulus( 0 ),
   m_minObjFuncValChange( 0.0 ),
   m_c1( 1e-4 ),
   m_rho( 0.1 )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// solve
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVector LineSearchGradDescOptimiser::solve( RealVector startValue ) const
{
   RealVector& solutionHypothesis = startValue;

   double objFuncValChange = std::numeric_limits< double >::max();
   double objFuncVal = m_objFunc.evaluate( solutionHypothesis );

   for ( size_t iIter = 0; iIter < m_maxIterations; ++iIter )
   {
      getLogger() << Msg::Debug << "Iteration " << iIter << ", Objective function value = " << objFuncVal << Msg::EndReq;

      const RealVector& gradient = m_objFunc.calculateGradient( solutionHypothesis );

      double gradientModulus = gradient * gradient;
      double alpha = 1;

      if ( gradientModulus < m_minGradModulus )
      {
         getLogger() << Msg::Debug << "Gradient convergence criterion met." << Msg::EndReq;
         break;
      }
      if ( objFuncValChange < m_minObjFuncValChange )
      {
         getLogger() << Msg::Debug << "Minimum objective function value change criterion met." << Msg::EndReq;
         break;
      }

      size_t nLineSearchIter = 0;
      while ( true )
      {
         const RealVector& x = solutionHypothesis - gradient * alpha;
         double lhs = m_objFunc.evaluate( x );
         double rhs = objFuncVal + m_c1 * alpha * gradientModulus;

         getLogger() << Msg::Verbose << "LHS = " << lhs << ", RHS = " << rhs << Msg::EndReq;

         if ( lhs <= rhs )
         {
            solutionHypothesis = x;
            break;
         }

         alpha *= m_rho;
         ++nLineSearchIter;
      }

      getLogger() << Msg::Debug << "Number of line search iterations needed = " << nLineSearchIter << Msg::EndReq;

      double oldObjFuncVal = objFuncVal;
      objFuncVal = m_objFunc.evaluate( solutionHypothesis );
      if ( oldObjFuncVal > objFuncVal )
      {
         objFuncValChange = oldObjFuncVal - objFuncVal;
      }
   }

   return solutionHypothesis;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setMaxIterations
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LineSearchGradDescOptimiser::setMaxIterations( size_t maxIterations )
{
   m_maxIterations = maxIterations;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setMinGradLength
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LineSearchGradDescOptimiser::setMinGradLength( double minGradLength )
{
   m_minGradModulus = minGradLength * minGradLength;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setMinObjFuncValChange
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LineSearchGradDescOptimiser::setMinObjFuncValChange( double minObjFuncValChange )
{
   m_minObjFuncValChange = minObjFuncValChange;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setWolfeConstant1
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LineSearchGradDescOptimiser::setWolfeConstant1( double c1 )
{
   m_c1 = c1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setContractionFactor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LineSearchGradDescOptimiser::setContractionFactor( double rho )
{
   m_rho = rho;
}

} /// namespace Math
