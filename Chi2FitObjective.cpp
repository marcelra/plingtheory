#include "Chi2FitObjective.h"

namespace Math
{

IFitFunction::IFitFunction( size_t numParameters ) :
   m_parameters( numParameters )
{}

IFitFunction::~IFitFunction()
{}

size_t IFitFunction::getNumParameters() const
{
   return m_parameters.size();
}

double IFitFunction::par( size_t parIndex ) const
{
   return m_parameters[ parIndex ];
}

void IFitFunction::setParameters( const RealVector& parameters )
{
   assert( parameters.size() == m_parameters.size() );
   m_parameters = parameters;
}

double IFitFunction::operator()( double x ) const
{
   return par( 0 ) + par( 1 ) * x;
}

IRealFunction* IFitFunction::clone() const
{
   return new IFitFunction( *this );
}



Chi2FitObjective::Chi2FitObjective( const RealVector& xData, const RealVector& yData, const RealVector& ySigma2, IFitFunction* function ) :
   m_xData( xData ),
   m_yData( yData ),
   m_ySigma2( ySigma2 ),
   m_func( function )
{
   assert( m_xData.size() == m_yData.size() );
   assert( m_xData.size() == m_ySigma2.size() );
}

double Chi2FitObjective::evaluate( const RealVector& x ) const
{
   m_func->setParameters( x );

   double chi2 = 0;
   for ( size_t iPoint = 0; iPoint < m_xData.size(); ++iPoint )
   {
      double diff = (*m_func)( m_xData[ iPoint ] ) - m_yData[ iPoint ];
      chi2 += ( diff * diff ) / m_ySigma2[ iPoint ];
   }
   return chi2;
}

size_t Chi2FitObjective::getNumParameters() const
{
   return m_func->getNumParameters();
}

} /// namespace Math
