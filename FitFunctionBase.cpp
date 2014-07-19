#include "FitFunctionBase.h"

namespace Math
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FitFunctionBase::FitFunctionBase( size_t numParameters ) :
   m_parameters( numParameters )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FitFunctionBase::~FitFunctionBase()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getNumParameters
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t FitFunctionBase::getNumParameters() const
{
   return m_parameters.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// par
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double FitFunctionBase::par( size_t parIndex ) const
{
   return m_parameters[ parIndex ];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setParameters
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FitFunctionBase::setParameters( const RealVector& parameters )
{
   assert( parameters.size() == m_parameters.size() );
   m_parameters = parameters;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// operator()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double FitFunctionBase::operator()( double x ) const
{
   return par( 0 ) + par( 1 ) * x;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// clone
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IRealFunction* FitFunctionBase::clone() const
{
   return new FitFunctionBase( *this );
}

} /// namespace Math
