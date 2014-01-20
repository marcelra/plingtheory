#include "WindowFunction.h"

#include <cmath>

namespace WaveAnalysis
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// WindowFunction base class implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WindowFunction::WindowFunction( size_t numSamples ) :
   m_numSamples( numSamples )
{}

WindowFunction::~WindowFunction()
{}

size_t WindowFunction::getSize() const
{
   return m_numSamples;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// RectangularWindowFunction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RectangularWindowFunction::RectangularWindowFunction( size_t numSamples ) :
   WindowFunction( numSamples )
{
   init();
}

void RectangularWindowFunction::init()
{
   /// Nothing to do
}

double RectangularWindowFunction::calc( size_t /*iSample*/ ) const
{
   return 1;
}

WindowFunction* RectangularWindowFunction::clone() const
{
   return new RectangularWindowFunction( *this );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// HannPoissonWindowFunction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HannPoissonWindowFunction::HannPoissonWindowFunction( size_t numSamples ) :
   WindowFunction( numSamples )
{
   init();
}

void HannPoissonWindowFunction::init()
{
   assert( m_numSamples > 1 );
   m_cachedValues.resize( m_numSamples );
   double alpha = 2;
   double nMinOne = m_numSamples - 1;
   double denom = 1.0 / ( nMinOne );
   for ( size_t i = 0; i < m_numSamples; ++i )
   {
      double twoI= i*2;
      double expArg = -alpha * fabs( nMinOne - twoI ) * denom;
      double fac = 0.5 * ( 1.0 - cos( twoI * M_PI * denom ) );
      m_cachedValues[i] = fac * exp( expArg );
   }
}

double HannPoissonWindowFunction::calc( size_t iSample ) const
{
   return m_cachedValues[ iSample ];
}

WindowFunction* HannPoissonWindowFunction::clone() const
{
   return new HannPoissonWindowFunction( *this );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// HanningWindowFunction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HanningWindowFunction::HanningWindowFunction( size_t numSamples ) :
   WindowFunction( numSamples )
{
   init();
}

void HanningWindowFunction::init()
{
   assert( m_numSamples > 1 );
   m_cachedValues.resize( m_numSamples );
   double cosArgFac = 2 * M_PI / m_numSamples;
   double centre = m_numSamples / 2.0;
   for ( size_t iWinSample = 0; iWinSample < m_numSamples; ++iWinSample )
   {
      double x = iWinSample - centre;
      double cosArg = x * cosArgFac;
      m_cachedValues[iWinSample] = 0.54 + 0.46*cos( cosArg );
   }
}

double HanningWindowFunction::calc( size_t iSample ) const
{
   return m_cachedValues[ iSample ];
}

WindowFunction* HanningWindowFunction::clone() const
{
   return new HanningWindowFunction( *this );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// HanningDerivativeWindowFunction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HanningDerivativeWindowFunction::HanningDerivativeWindowFunction( size_t numSamples ) :
   WindowFunction( numSamples )
{
   init();
}

void HanningDerivativeWindowFunction::init()
{
   assert( m_numSamples > 1 );
   m_cachedValues.resize( m_numSamples );
   double sinArgFac = 2 * M_PI / m_numSamples;
   double centre = m_numSamples / 2.0;
   for ( size_t iWinSample = 0; iWinSample < m_numSamples; ++iWinSample )
   {
      double x = iWinSample - centre;
      double sinArg = x * sinArgFac;
      m_cachedValues[iWinSample] = 0.46*sin( sinArg ) * sinArgFac;
   }
}

double HanningDerivativeWindowFunction::calc( size_t iSample ) const
{
   return m_cachedValues[ iSample ];
}

WindowFunction* HanningDerivativeWindowFunction::clone() const
{
   return new HanningDerivativeWindowFunction( *this );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// HanningTimeRampedWindowFunction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HanningTimeRampedWindowFunction::HanningTimeRampedWindowFunction( size_t numSamples ) :
   WindowFunction( numSamples )
{
   init();
}

void HanningTimeRampedWindowFunction::init()
{
   assert( m_numSamples > 1 );
   m_cachedValues.resize( m_numSamples );
   double cosArgFac = 2 * M_PI / m_numSamples;
   double centre = m_numSamples / 2.0;
   for ( size_t iWinSample = 0; iWinSample < m_numSamples; ++iWinSample )
   {
      double x = iWinSample - centre;
      double cosArg = x * cosArgFac;
      m_cachedValues[iWinSample] = ( 0.54 + 0.46*cos( cosArg ) ) * x;
   }
}

double HanningTimeRampedWindowFunction::calc( size_t iSample ) const
{
   return m_cachedValues[ iSample ];
}

WindowFunction* HanningTimeRampedWindowFunction::clone() const
{
   return new HanningTimeRampedWindowFunction( *this );
}


} /// namespace WaveAnalysis
