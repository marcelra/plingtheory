#include "IPdf.h"

#include <limits>

#include "RealMemFunction.h"

namespace Math
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IPdf::~IPdf()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMinX
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double IPdf::getMinX() const
{
   return -std::numeric_limits< double >::max();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMaxX
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double IPdf::getMaxX() const
{
   return std::numeric_limits< double >::max();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getDensityAsFunc
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IRealFunction::Ptr IPdf::getDensityAsFunc() const
{
   RealMemFunction< IPdf >* rmf = new RealMemFunction< IPdf >( &IPdf::getDensity, this );
   return IRealFunction::Ptr( rmf );
}

} /// namespace Math
