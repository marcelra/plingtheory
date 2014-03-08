#include "PcSetDrawAttr.h"

#include "PaintArea.h"

#include <QPainter>

namespace Plotting
{

PcSetDrawAttr::PcSetDrawAttr( QColor lineColor, double lineWidth, bool antialiasing ) :
   m_pen( lineColor,lineWidth ),
   m_brush( Qt::NoBrush ),
   m_antialiasing( antialiasing )
{}

PcSetDrawAttr::PcSetDrawAttr( QColor lineColor, double lineWidth, QColor fillColor, Qt::BrushStyle fillStyle, bool antialiasing ) :
   m_pen( lineColor, lineWidth ),
   m_brush( fillColor, fillStyle ),
   m_antialiasing( antialiasing )
{}

PcSetDrawAttr::~PcSetDrawAttr()
{}

void PcSetDrawAttr::execute( PaintArea& paintArea ) const
{
   paintArea.getPainter().setPen( m_pen );
   paintArea.getPainter().setBrush( m_brush );
   paintArea.getPainter().setRenderHint( QPainter::Antialiasing, m_antialiasing );
}

} /// namespace Plotting
