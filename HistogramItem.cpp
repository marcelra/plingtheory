#include "HistogramItem.h"

namespace Plotting
{

HistogramItem::HistogramItem( const Math::IAccumArray& data ) :
   CurveItem()
{
   size_t nDataPoints = data.getNumBins() * 2 + 2;
   std::vector< double > xData( nDataPoints );
   std::vector< double > yData( nDataPoints );

   if ( data.getNumBins() > 0 )
   {
      xData[ 0 ] = data.getBin( 0 ).getMinX();
      yData[ 0 ] = 0;

      size_t iPoint = 1;
      for ( size_t iBin = 0; iBin < data.getNumBins(); ++iBin )
      {
         const Math::IAccumArray::Bin& bin = data.getBin( iBin );
         xData[ iPoint ] = bin.getMinX();
         yData[ iPoint ] = bin.getContents();

         ++iPoint;

         xData[ iPoint ] = bin.getMaxX();
         yData[ iPoint ] = bin.getContents();

         ++iPoint;
      }

      assert( iPoint == nDataPoints - 1 );

      xData[ iPoint ] = data.getBin( data.getNumBins() - 1 ).getMaxX();
      yData[ iPoint ] = 0;
   }

   m_data = YVsXData( xData, yData );
}

} /// namespace Plotting
