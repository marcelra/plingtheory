#ifndef REALFUNCTIONPTR_H
#define REALFUNCTIONPTR_H

#include "IRealFunction.h"

namespace Math
{

class RealFunctionPtr : public IRealFunction
{
   public:
      RealFunctionPtr( double ( *func )( double ) );

      double operator()( double x ) const;
      RealFunctionPtr* clone() const;

   private:
      double ( *m_func )( double );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// operator()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double RealFunctionPtr::operator()( double x ) const
{
   return ( *m_func )( x );
}

} /// namespace Math

#endif // REALFUNCTIONPTR_H
