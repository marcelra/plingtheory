#include "PolynomialFitFunction.h"

namespace Math
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PolynomialFitFunction::PolynomialFitFunction( size_t degree ) :
   FitFunctionBase( degree + 1 )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// operator()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double PolynomialFitFunction::operator()( double x ) const
{
   double result = par( 0 );
   double monomial = 1;
   for ( size_t i = 1; i < getNumParameters(); ++i )
   {
      monomial *= x;
      result += par( i ) * monomial;
   }
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// clone
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IRealFunction* PolynomialFitFunction::clone() const
{
   return new PolynomialFitFunction( *this );
}

} /// namespace Math
