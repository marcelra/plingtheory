#include "RegLargeDataCurve.h"

#include "PaintArea.h"
#include "PcLinePaint.h"
#include "PcSetDrawAttr.h"

#include <cmath>

namespace Plotting
{

RegLargeDataCurve::RegLargeDataCurve( const std::vector< double >& xData, const std::vector< double >& yData, size_t numPointsLowResCache ) :
   IYVsXItem( xData, yData )
{
   assert( xData.size() > numPointsLowResCache );

   std::vector< double > lowResX( numPointsLowResCache );
   std::vector< double > lowResY( numPointsLowResCache );

   double step = static_cast< double >( xData.size() ) / numPointsLowResCache;
   double indexDouble = 0;
   size_t nextIndex = 0;


   for ( size_t i = 0; i < lowResX.size(); ++i )
   {
      size_t index = nextIndex;
      indexDouble += step;
      nextIndex = static_cast< size_t >( indexDouble );

      double yMax = 0;
      size_t iMaxSearch = index;
      size_t iMaxFound = 0;
      while ( iMaxSearch < nextIndex )
      {
         if ( fabs( yData[ iMaxSearch ] ) > yMax )
         {
            yMax = fabs( yData[ iMaxSearch ] );
            iMaxFound = iMaxSearch;
         }
         ++iMaxSearch;
      }

      if ( iMaxFound == 0 )
      {
         lowResX[ i ] = xData[ index ];
         lowResY[ i ] = yData[ index ];
      }
      else
      {
         lowResX[ i ] = xData[ iMaxFound ];
         lowResY[ i ] = yData[ iMaxFound ];
      }
   }

   m_lowResCache = new YVsXData( lowResX, lowResY );

   m_numPointsCache = numPointsLowResCache;
}

RegLargeDataCurve::~RegLargeDataCurve()
{
   delete m_lowResCache;
}

void RegLargeDataCurve::generatePlotCommands( PaintArea *paintArea ) const
{
   const std::vector< double >& xData = m_data.getX();
   double dist = xData[ 1 ] - xData[ 0 ];
   size_t firstIndex = ( paintArea->getViewport().left() - getMinX() ) / dist;
   size_t lastIndex = ( paintArea->getViewport().right() - getMinX() ) / dist + 1;
   firstIndex = paintArea->getViewport().left() > getMinX() ? firstIndex : 0;
   lastIndex = lastIndex < xData.size() ? lastIndex : xData.size();

   size_t nSamplesDraw = lastIndex - firstIndex;


   PcSetDrawAttr* drawAttrCmd = new PcSetDrawAttr( getLineColor(), getLineWidth(), getAntialiasing() );
   paintArea->addPaintCommand( drawAttrCmd );

   if ( nSamplesDraw > m_numPointsCache )
   {
      generateLowResCommands( paintArea );
   }
   else
   {
      generateHiResCommands( paintArea );
   }
}

void RegLargeDataCurve::generateLowResCommands( PaintArea* paintArea ) const
{
   const std::vector< double >& xData = m_lowResCache->getX();
   const std::vector< double >& yData = m_lowResCache->getY();

   for ( size_t i = 0; i < xData.size() - 1; ++i )
   {
      PcLinePaint* linePaint = new PcLinePaint( QPointF( xData[ i ], yData[ i ] ), QPointF( xData[ i + 1 ], yData[ i + 1 ] ) );
      paintArea->addPaintCommand( linePaint );
   }
}

void RegLargeDataCurve::generateHiResCommands( PaintArea* paintArea ) const
{
   const std::vector< double >& xData = m_data.getX();
   const std::vector< double >& yData = m_data.getY();

   double dist = xData[ 1 ] - xData[ 0 ];

   size_t firstIndex = ( paintArea->getViewport().left() - getMinX() ) / dist;
   size_t lastIndex = ceil( ( paintArea->getViewport().right() - getMinX() ) / dist ) + 1;
   firstIndex = paintArea->getViewport().left() > getMinX() ? firstIndex : 0;
   lastIndex = lastIndex < xData.size() ? lastIndex : xData.size();

   for ( size_t i = firstIndex; i < lastIndex - 1; ++i )
   {
      PcLinePaint* linePaint = new PcLinePaint( QPointF( xData[ i ], yData[ i ] ), QPointF( xData[ i + 1 ], yData[ i + 1 ] ) );
      paintArea->addPaintCommand( linePaint );
   }
}

} /// namespace Plotting
