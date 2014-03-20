#include "NewtonSolver1D.h"

#include <cmath>

namespace Math
{

NewtonSolver1D::NewtonSolver1D( const IRealFuncWithDerivative& function, double value ) :
   m_func( function ),
   m_tolerance( 1e-6 ),
   m_value( value )
{
}

NewtonSolver1D::Result NewtonSolver1D::solve( double startValue, size_t nIterationsMax )
{
   double x = startValue;

   double oldF = 0;

   for ( size_t iIter = 0; iIter < nIterationsMax; ++iIter )
   {
      double f = m_func( x ) - m_value;
      double df = m_func.deriv( x );

      double deltaX = -f / df;
      x += deltaX;

      if ( fabs( f - oldF ) < m_tolerance )
      {
         break;
      }
   }

   Result result;
   result.setSolution( x );
   return result;
}




void NewtonSolver1D::Result::setSolution( double solution )
{
   m_solution = solution;
}

double NewtonSolver1D::Result::getSolution() const
{
   return m_solution;
}

} /// namespace Math
