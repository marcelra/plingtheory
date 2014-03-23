#include "RealFunctionPtr.h"

namespace Math
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealFunctionPtr::RealFunctionPtr( double ( *func )( double ) ) :
   m_func( func )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// clone
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealFunctionPtr* RealFunctionPtr::clone() const
{
   return new RealFunctionPtr( *this );
}

} /// namespace Math