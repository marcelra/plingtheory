#include "BisectionSolver1D.h"

#include <cmath>

namespace
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// sign (anonymous namespace)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int sign( double x )
{
   if ( x < 0 )
   {
      return -1;
   }
   else
   {
      return 1;
   }
}


} /// anonymous namespace

namespace Math
{


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BisectionSolver1D::BisectionSolver1D()
{
   m_solutionTolerance = 1e-3;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// solve
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BisectionSolver1D::Result BisectionSolver1D::solve( const IRealFunction& function, double value, const Interval& interval )
{
   Result result;

   double fLeft = function( interval.getMin() ) - value;
   double fRight = function( interval.getMax() ) - value;

   if ( sign( fLeft ) == sign( fRight ) )
   {
      result.m_status = Result::Failed;
      return result;
   }

   Interval workingInterval = interval;
   for ( size_t iter = 0; iter < m_maxIterations; ++iter )
   {
      double centre = workingInterval.getMin() + 0.5 * workingInterval.getLength();
      double fCentre = function( centre ) - value;

      if ( sign( fLeft ) == sign( fCentre ) )
      {
         workingInterval = Interval( centre, workingInterval.getMax() );
      }
      else
      {
         workingInterval = Interval( workingInterval.getMin(), centre );
      }

      if ( fabs( fCentre ) < m_solutionTolerance )
      {
         result.m_solution = centre;
         result.m_eps = fabs( fCentre );
         result.m_status = Result::Converged;
         break;
      }
   }

   return result;
}

} /// namespace Math
