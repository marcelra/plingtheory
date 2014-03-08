#include "GridItem.h"

#include "AxisPaintArea.h"
#include "PaintArea.h"
#include "PcLinePaint.h"
#include "PcSetDrawAttr.h"

#include <QPainter>

namespace Plotting
{

GridItem::GridItem( const AxisPaintArea& xAxis, const AxisPaintArea& yAxis ) :
   m_xAxis( xAxis ),
   m_yAxis( yAxis )
{}

GridItem::~GridItem()
{}

void GridItem::generatePlotCommands( PaintArea* paintArea ) const
{
   PcSetDrawAttr* drawAttrCmd = new PcSetDrawAttr( getLineColor(), getLineWidth(), getAntialiasing() );
   paintArea->addPaintCommand( drawAttrCmd );

   double yMin = m_yAxis.getMin();
   double yMax = m_yAxis.getMax();

   const std::vector< double >& xTicks = m_xAxis.getMajorTicks();
   for ( size_t iTick = 0; iTick < xTicks.size(); ++iTick )
   {
      PcLinePaint* line = new PcLinePaint( QPointF( xTicks[ iTick ], yMin ), QPointF( xTicks[ iTick ], yMax ) );
      paintArea->addPaintCommand( line );
   }

   double xMin = m_xAxis.getMin();
   double xMax = m_xAxis.getMax();

   const std::vector< double >& yTicks = m_yAxis.getMajorTicks();
   for ( size_t iTick = 0; iTick < yTicks.size(); ++iTick )
   {
      PcLinePaint* line = new PcLinePaint( QPointF( xMin, yTicks[ iTick ] ), QPointF( xMax, yTicks[ iTick ] ) );
      paintArea->addPaintCommand( line );
   }

   PcSetDrawAttr* axisDrawAttrCmd = new PcSetDrawAttr( QColor( Qt::black ), 2, true );
   paintArea->addPaintCommand( axisDrawAttrCmd );

   PcLinePaint* xAxisGrid = new PcLinePaint( QPointF( xMin, 0 ), QPointF( xMax, 0 ) );
   paintArea->addPaintCommand( xAxisGrid );
   PcLinePaint* yAxisGrid = new PcLinePaint( QPointF( 0, yMin ), QPointF( 0, yMax ) );
   paintArea->addPaintCommand( yAxisGrid );
}

double GridItem::getMinX() const
{
   return m_xAxis.getMin();
}

double GridItem::getMaxX() const
{
   return m_xAxis.getMax();
}

double GridItem::getMinY() const
{
   return m_yAxis.getMin();
}

double GridItem::getMaxY() const
{
   return m_yAxis.getMax();
}

} /// namespace Plotting
