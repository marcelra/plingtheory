#include "FitFunctionBase.h"

namespace Math
{

FitFunctionBase::FitFunctionBase( size_t numParameters ) :
   m_parameters( numParameters )
{}

FitFunctionBase::~FitFunctionBase()
{}

size_t FitFunctionBase::getNumParameters() const
{
   return m_parameters.size();
}

double FitFunctionBase::par( size_t parIndex ) const
{
   return m_parameters[ parIndex ];
}

void FitFunctionBase::setParameters( const RealVector& parameters )
{
   assert( parameters.size() == m_parameters.size() );
   m_parameters = parameters;
}

double FitFunctionBase::operator()( double x ) const
{
   return par( 0 ) + par( 1 ) * x;
}

IRealFunction* FitFunctionBase::clone() const
{
   return new FitFunctionBase( *this );
}

} /// namespace Math
