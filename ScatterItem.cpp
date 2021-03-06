#include "ScatterItem.h"

#include "PaintArea.h"
#include "PcMarkerPaint.h"
#include "PcSetDrawAttr.h"

#include <cstddef>

namespace Plotting
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ScatterItem::ScatterItem( const std::vector< double >& xData, const std::vector< double >& yData, const MarkerDrawAttr& markerDrawAttr ) :
   IYVsXItem( xData, yData ),
   MarkerDrawAttr( markerDrawAttr )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ScatterItem::~ScatterItem()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// generatePlotCommands
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ScatterItem::generatePlotCommands( PaintArea* paintArea ) const
{
   PcSetDrawAttr* drawAttrCmd = new PcSetDrawAttr( getLineColor(), getLineWidth(), getAntialiasing() );
   paintArea->addPaintCommand( drawAttrCmd );

   const std::vector< double >& xData = m_data.getX();
   const std::vector< double >& yData = m_data.getY();

   const QRectF& viewport = paintArea->getViewport();

   const MarkerType markerType = getMarkerType();

   for ( size_t i = 0; i < xData.size(); ++i )
   {
      const QPointF point( xData[ i ], yData[ i ] );
      if ( viewport.contains( point ) )
      {
         PcMarkerPaint* cmd = new PcMarkerPaint( QPointF( xData[ i ], yData[ i ] ), getMarkerSize(), markerType );
         paintArea->addPaintCommand( cmd );
      }
   }
}

} /// namespace Plotting
