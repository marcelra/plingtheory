#include "MarkerDrawAttr.h"

namespace Plotting
{

MarkerDrawAttr::MarkerDrawAttr( QColor color, MarkerType markerType, double width, bool antialiasing ) :
   LineDrawAttr( color, width, antialiasing ),
   m_markerType( markerType )
{}

void MarkerDrawAttr::setMarkerType( Plotting::MarkerType markerType )
{
   m_markerType = markerType;
}

MarkerType MarkerDrawAttr::getMarkerType() const
{
   return m_markerType;
}

} /// namespace Plotting
