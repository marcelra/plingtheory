#include "PcLinePaint.h"

#include "PaintArea.h"

#include <QPainter>

namespace Plotting
{

PcLinePaint::PcLinePaint( const QPointF& v0, const QPointF& v1 ) :
   m_v0( v0 ),
   m_v1( v1 )
{}

PcLinePaint::~PcLinePaint()
{}

void PcLinePaint::execute( PaintArea& paintArea ) const
{
   const QPointF& v0_transformed = paintArea.transformToCanvasCoordinates( m_v0 );
   const QPointF& v1_transformed = paintArea.transformToCanvasCoordinates( m_v1 );
   QLineF line( v0_transformed, v1_transformed );
   paintArea.getPainter().drawLine( line );
}

} /// namespace Plotting
