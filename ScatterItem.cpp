#include "ScatterItem.h"

#include "PaintArea.h"
#include "PcMarkerPaint.h"
#include "PcSetDrawAttr.h"

#include <cstddef>

namespace Plotting
{

ScatterItem::ScatterItem( const std::vector< double >& xData, const std::vector< double >& yData ) :
   IYVsXItem( xData, yData )
{}

ScatterItem::~ScatterItem()
{}

void ScatterItem::generatePlotCommands( PaintArea* paintArea ) const
{
   PcSetDrawAttr* drawAttrCmd = new PcSetDrawAttr( getLineColor(), getLineWidth(), getAntialiasing() );
   paintArea->addPaintCommand( drawAttrCmd );

   const std::vector< double >& xData = m_data.getX();
   const std::vector< double >& yData = m_data.getY();

   const MarkerType markerType = getMarkerType();

   for ( size_t i = 0; i < xData.size(); ++i )
   {
      PcMarkerPaint* cmd = new PcMarkerPaint( QPointF( xData[ i ], yData[ i ] ), 4, markerType );
      paintArea->addPaintCommand( cmd );
   }
}

} /// namespace Plotting
