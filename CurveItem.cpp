#include "CurveItem.h"

#include "PaintArea.h"
#include "PcLinePaint.h"
#include "PcSetDrawAttr.h"

#include <cassert>
#include <cstddef>

namespace Plotting
{

CurveItem::CurveItem( const std::vector< double >& xData, const std::vector< double >& yData ) :
   IYVsXItem( xData, yData )
{}

CurveItem::~CurveItem()
{}

void CurveItem::generatePlotCommands( PaintArea* paintArea ) const
{
   PcSetDrawAttr* drawAttrCmd = new PcSetDrawAttr( getLineColor(), getLineWidth(), getAntialiasing() );
   paintArea->addPaintCommand( drawAttrCmd );

   const std::vector< double >& xData = m_data.getX();
   const std::vector< double >& yData = m_data.getY();

   for ( size_t i = 0; i < xData.size() - 1; ++i )
   {
      PcLinePaint* linePaint = new PcLinePaint( QPointF( xData[ i ], yData[ i ] ), QPointF( xData[ i + 1], yData[ i + 1 ] ) );
      paintArea->addPaintCommand( linePaint );
   }
}

} /// namespace Plotting
