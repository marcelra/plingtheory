#include "ZScatterItem.h"

#include "PaintArea.h"
#include "PcMarkerPaint.h"
#include "PcSetDrawAttr.h"

#include <cassert>

namespace Plotting
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZScatterItem::ZScatterItem( const std::vector< double >& xData, const std::vector< double >& yData, const std::vector< double >& zData, const Palette& palette, const MarkerDrawAttr& markerDrawAttr ) :
   IYVsXItem( xData, yData ),
   MarkerDrawAttr( markerDrawAttr ),
   m_palette( palette ),
   m_zData( zData ),
   m_zMin( std::numeric_limits< double >::max() ),
   m_zMax( -std::numeric_limits< double >::max() )
{
   assert( m_zData.size() == m_data.getX().size() );

   for ( size_t i = 0; i < m_zData.size(); ++i )
   {
      if ( m_zData[ i ] < m_zMin )
      {
         m_zMin = m_zData[ i ];
      }
      if ( m_zData[ i ] > m_zMax )
      {
         m_zMax = m_zData[ i ];
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// generatePlotCommands
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ZScatterItem::generatePlotCommands( PaintArea* paintArea ) const
{
   double zTot = m_zMax - m_zMin;

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
         double z = m_zData[ i ];
         PcSetDrawAttr* setColourCmd = new PcSetDrawAttr( m_palette.getColour( ( z - m_zMin ) / zTot ), getLineWidth() );
         paintArea->addPaintCommand( setColourCmd );
         PcMarkerPaint* cmd = new PcMarkerPaint( QPointF( xData[ i ], yData[ i ] ), getMarkerSize(), markerType );
         paintArea->addPaintCommand( cmd );
      }
   }
}

} /// namespace Plotting
