#include "NewtonSolver1D.h"

#include <cmath>

namespace Math
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
NewtonSolver1D::NewtonSolver1D()
{
   m_solutionTolerance = 1e-12;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// solve
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
NewtonSolver1D::Result NewtonSolver1D::solve( const IRealFuncWithDerivative& function, double value, double startX )
{
   Result result;

   double x = startX;
   double f = function( x ) - value;

   for ( size_t iIter = 0; iIter < m_maxIterations; ++iIter )
   {
      double df = function.deriv( x );

      if ( df < s_failDerivThreshold )
      {
         result.m_status = Result::Failed;
         break;
      }

      double deltaX = -f / df;

      x += deltaX;

      f = function( x ) - value;

      if ( fabs( f ) < m_solutionTolerance )
      {
         result.m_status = Result::Converged;
         break;
      }
   }

   result.m_solution = x;
   result.m_eps = f;
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Static members
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double NewtonSolver1D::s_failDerivThreshold = 1e-14;




} /// namespace Math
