#include "KernelPdf.h"

#include <cmath>

#include <iostream>

namespace Math
{

KernelPdf::KernelPdf( IPdf::CPtr kernel, const RealVector& sampling ) :
   m_kernel( kernel.release() ),
   m_sampling( sampling ),
   m_weights( sampling.size(), 1.0 / m_sampling.size() )
{
   assert( fabs( sum( m_weights ) - 1 ) < 1e-12 );
}

KernelPdf::KernelPdf( IPdf::CPtr kernel, const RealVector& sampling, const RealVector& weights ) :
   m_kernel( kernel.release() ),
   m_sampling( sampling ),
   m_weights( weights )
{
   assert( weights.size() == sampling.size() );
   double norm = 1.0 / sum( m_weights );
   scale( m_weights, norm );
}

double KernelPdf::getDensity( double x ) const
{
   double val = 0;
   for ( size_t i = 0; i < m_sampling.size(); ++i )
   {
      val += m_weights[ i ] * m_kernel->getDensity( x - m_sampling[ i ] );
   }
   return val;
}

double KernelPdf::getProbability( double x0, double x1 ) const
{
   return getIntegral( x1 ) - getIntegral( x0 );
}

double KernelPdf::getIntegral( double x ) const
{
   double integral = 0;
   for ( size_t i = 0; i < m_sampling.size(); ++i )
   {
      integral += m_weights[ i ] * m_kernel->getIntegral( x - m_sampling[ i ] );
   }
   return integral;
}

} /// namespace Math

