#include "RegLargeDataCurve.h"

#include "PaintArea.h"
#include "PcLinePaint.h"
#include "PcSetDrawAttr.h"

#include <cmath>

namespace Plotting
{

RegLargeDataCurve::RegLargeDataCurve( const std::vector< double >& xData, const std::vector< double >& yData ) :
   IYVsXItem( xData, yData )
{}

RegLargeDataCurve::~RegLargeDataCurve()
{
}

/// TODO: Needs cleanup
void RegLargeDataCurve::generatePlotCommands( PaintArea *paintArea ) const
{
   const QRectF& viewport = paintArea->getViewport();

   PcSetDrawAttr* drawAttrCmd = new PcSetDrawAttr( getLineColor(), getLineWidth(), getAntialiasing() );
   paintArea->addPaintCommand( drawAttrCmd );

   double xSpacing = ( getMaxX() - getMinX() ) / m_data.getSize();

   size_t firstVisiblePoint = 0;
   size_t lastVisiblePoint = m_data.getSize();

   if ( viewport.left() > getMinX() )
   {
      firstVisiblePoint = ( viewport.left() - getMinX() ) / xSpacing;
   }
   if ( viewport.right() < getMaxX() )
   {
      lastVisiblePoint = ( m_data.getSize() - ( getMaxX() - viewport.right() ) / xSpacing ) + 1;
      lastVisiblePoint = lastVisiblePoint > m_data.getSize() ? m_data.getSize() : lastVisiblePoint;
   }

   size_t nPointsDraw = lastVisiblePoint - firstVisiblePoint;

   int compressionFactor = log ( nPointsDraw / paintArea->getCanvas().width() ) / log( 2 );

   const YVsXData& dataDraw = getData( compressionFactor );
   const std::vector< double >& xDataDraw = dataDraw.getX();
   const std::vector< double >& yDataDraw = dataDraw.getY();

   xSpacing = ( dataDraw.getMaxX() - dataDraw.getMinX() ) / dataDraw.getSize();

   firstVisiblePoint = 0;
   lastVisiblePoint = dataDraw.getSize();
   if ( viewport.left() > dataDraw.getMinX() )
   {
      firstVisiblePoint = ( viewport.left() - dataDraw.getMinX() ) / xSpacing;
   }
   if ( viewport.right() < getMaxX() )
   {
      lastVisiblePoint = ( dataDraw.getSize() - ( dataDraw.getMaxX() - viewport.right() ) / xSpacing ) + 1;
   }
   lastVisiblePoint = lastVisiblePoint > dataDraw.getSize() ? dataDraw.getSize() : lastVisiblePoint;

   for ( size_t i = firstVisiblePoint; i < lastVisiblePoint - 1; ++i )
   {
      PcLinePaint* linePaint = new PcLinePaint( QPointF( xDataDraw[ i ], yDataDraw[ i ] ), QPointF( xDataDraw[ i + 1], yDataDraw[ i + 1 ] ) );
      paintArea->addPaintCommand( linePaint );
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getData
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const YVsXData& RegLargeDataCurve::getData( int compressionFactor ) const
{
   if ( compressionFactor <= 1 )
   {
      return m_data;
   }
   else
   {
      if ( m_cache[ compressionFactor ].getSize() == 0 )
      {
         createLowResDataSet( compressionFactor );
      }
      return m_cache[ compressionFactor ];
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// createLowResDataSet
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RegLargeDataCurve::createLowResDataSet( int compressionFactor ) const
{
   std::vector< double > xData;
   std::vector< double > yData;

   size_t findMaxInterval = 2 << compressionFactor;

   xData.reserve( 2 * m_data.getSize() / findMaxInterval );
   yData.reserve( 2 * m_data.getSize() / findMaxInterval );

   double step = m_data.getX()[ findMaxInterval ] - m_data.getX()[ 0 ];

   for ( size_t i = 0; i < m_data.getSize() - findMaxInterval; ++i )
   {
      size_t iSample = i;
      double max = -std::numeric_limits< double >::max();
      double min = std::numeric_limits< double >::max();
      size_t maxIndex = 0;
      size_t minIndex = 0;
      for ( ; i < iSample + findMaxInterval; ++i )
      {
         double val = m_data.getY()[ i ];
         if ( val > max )
         {
            max = val;
            maxIndex = i;
         }
         if ( val < min )
         {
            min = val;
            minIndex = i;
         }
      }
      xData.push_back( m_data.getX()[ iSample ] );
      yData.push_back( minIndex < maxIndex ? min : max );
      xData.push_back( m_data.getX()[ iSample ] + step / 2 );
      yData.push_back( minIndex > maxIndex ? min : max );
   }

   m_cache[ compressionFactor ] = YVsXData( xData, yData );
}

} /// namespace Plotting
