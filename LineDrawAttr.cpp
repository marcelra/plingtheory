#include "LineDrawAttr.h"

namespace Plotting
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LineDrawAttr::LineDrawAttr( QColor color, double width, bool antialiasing ) :
   m_color( color ),
   m_width( width ),
   m_antialiasing( antialiasing )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LineDrawAttr::~LineDrawAttr()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setLineColor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LineDrawAttr::setLineColor( const QColor& color )
{
   m_color = color;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getLineColor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const QColor& LineDrawAttr::getLineColor() const
{
   return m_color;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setLineWidth
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LineDrawAttr::setLineWidth( double width )
{
   m_width = width;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getLineWidth
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double LineDrawAttr::getLineWidth() const
{
   return m_width;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setAntialiasing
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LineDrawAttr::setAntialiasing( bool antialiasing )
{
   m_antialiasing = antialiasing;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getAntialiasing
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool LineDrawAttr::getAntialiasing() const
{
   return m_antialiasing;
}

} /// namespace Plotting
