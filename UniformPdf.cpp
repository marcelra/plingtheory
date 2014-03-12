#include "UniformPdf.h"

#include <cassert>

namespace Math
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UniformPdf::UniformPdf( double min, double max ) :
   m_min( min ),
   m_max( max ),
   m_density( 1 / ( max - min ) )
{
   assert( max >  min );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// eval
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double UniformPdf::eval( double /*x*/ ) const
{
   return m_density;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getDensity
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double UniformPdf::getDensity( double x ) const
{
   if ( x < m_min || x > m_max )
   {
      return 0;
   }
   return eval( x );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getProbability
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double UniformPdf::getProbability( double x0, double x1 ) const
{
   x0 = x0 < m_min ? m_min : x0;
   x1 = x0 > m_max ? m_max : x1;
   return ( x1 - x0 ) * m_density;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMinX
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double UniformPdf::getMinX() const
{
   return m_min;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMaxX
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double UniformPdf::getMaxX() const
{
   return m_max;
}

} /// namespace Math
