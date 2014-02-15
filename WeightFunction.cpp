#include "WeightFunction.h"

#include <cmath>

namespace Math
{

IWeightFunction::~IWeightFunction()
{}

GaussianWeightFunction::GaussianWeightFunction( double min, double max, double sigma ) :
   m_min( min ),
   m_max( max ),
   m_argFac( 1.0 / ( 2 * sigma * sigma ) )
{
   m_norm = 1;
}

GaussianWeightFunction::~GaussianWeightFunction()
{}

double GaussianWeightFunction::getMin() const
{
   return m_min;
}

double GaussianWeightFunction::getMax() const
{
   return m_max;
}

double GaussianWeightFunction::getWeight( double x ) const
{
   return m_norm * exp( -x*x * m_argFac );
}

} /// namespace Math

