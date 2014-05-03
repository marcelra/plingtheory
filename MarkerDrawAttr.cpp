#include "MarkerDrawAttr.h"

namespace Plotting
{

MarkerDrawAttr::MarkerDrawAttr( QColor color, MarkerType markerType, double markerSize, double width, bool antialiasing ) :
   LineDrawAttr( color, width, antialiasing ),
   m_markerType( markerType ),
   m_markerSize( markerSize )
{}

void MarkerDrawAttr::setMarkerType( Plotting::MarkerType markerType )
{
   m_markerType = markerType;
}

void MarkerDrawAttr::setMarkerSize( double markerSize )
{
   m_markerSize = markerSize;
}

MarkerType MarkerDrawAttr::getMarkerType() const
{
   return m_markerType;
}

double MarkerDrawAttr::getMarkerSize() const
{
   return m_markerSize;
}

} /// namespace Plotting
