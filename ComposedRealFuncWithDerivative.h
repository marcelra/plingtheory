#ifndef COMPOSEDREALFUNCWITHDERIVATIVE_H
#define COMPOSEDREALFUNCWITHDERIVATIVE_H

#include "IRealFuncWithDerivative.h"

namespace Math
{

class ComposedRealFuncWithDerivative : public IRealFuncWithDerivative
{
   public:
      ComposedRealFuncWithDerivative( const IRealFunction& func, const IRealFunction& derivative );
      virtual ~ComposedRealFuncWithDerivative();

      ComposedRealFuncWithDerivative( const ComposedRealFuncWithDerivative& other );
      ComposedRealFuncWithDerivative& operator=( const ComposedRealFuncWithDerivative& other );

      ComposedRealFuncWithDerivative* clone() const;

      double operator()( double x ) const;
      double deriv( double x ) const;

   private:
      IRealFunction*       m_func;
      IRealFunction*       m_derivative;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods ComposedRealFuncWithDerivative
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// operator()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double ComposedRealFuncWithDerivative::operator()( double x ) const
{
   return ( *m_func )( x );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// deriv
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double ComposedRealFuncWithDerivative::deriv( double x ) const
{
   return ( *m_derivative )( x );
}


} /// namespace Math

#endif // COMPOSEDREALFUNCWITHDERIVATIVE_H
