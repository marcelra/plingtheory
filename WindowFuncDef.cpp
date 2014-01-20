#include "WindowFuncDef.h"

namespace WaveAnalysis
{

WindowFuncDef::~WindowFuncDef()
{}

WindowFunction* RectangularWindowFuncDef::createWindowFunction( size_t numSamples ) const
{
   return new RectangularWindowFunction( numSamples );
}

WindowFuncDef* RectangularWindowFuncDef::clone() const
{
   return new RectangularWindowFuncDef( *this );
}

WindowFunction* HannPoissonWindowFuncDef::createWindowFunction( size_t numSamples ) const
{
   return new HannPoissonWindowFunction( numSamples );
}

WindowFuncDef* HannPoissonWindowFuncDef::clone() const
{
   return new HannPoissonWindowFuncDef( *this );
}

WindowFunction* HanningWindowFuncDef::createWindowFunction( size_t numSamples ) const
{
   return new HanningWindowFunction( numSamples );
}

WindowFuncDef* HanningWindowFuncDef::clone() const
{
   return new HanningWindowFuncDef( *this );
}

WindowFunction* HanningDerivativeWindowFuncDef::createWindowFunction( size_t numSamples ) const
{
   return new HanningDerivativeWindowFunction( numSamples );
}

WindowFuncDef* HanningDerivativeWindowFuncDef::clone() const
{
   return new HanningDerivativeWindowFuncDef( *this );
}

WindowFunction* HanningTimeRampedWindowFuncDef::createWindowFunction( size_t numSamples ) const
{
   return new HanningTimeRampedWindowFunction( numSamples );
}

WindowFuncDef* HanningTimeRampedWindowFuncDef::clone() const
{
   return new HanningTimeRampedWindowFuncDef( *this );
}

} /// namespace WaveAnalysis
