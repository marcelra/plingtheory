#include "PredefinedRealFunctions.h"

#include <cmath>

namespace Math
{

Log10Function::Log10Function( double min ) :
   m_min( min )
{}

double Log10Function::operator()( double x ) const
{
   if ( x > m_min )
   {
      return log( x );
   }
   else
   {
      return log( m_min );
   }
}

IRealFunction* Log10Function::clone() const
{
   return new Log10Function( *this );
}

} /// namespace Math
