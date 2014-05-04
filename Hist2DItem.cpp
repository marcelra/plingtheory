#include "Hist2DItem.h"

#include "PaintArea.h"
#include "PcPixmapPaint.h"

#include <QColor>
#include <QPainter>

namespace Plotting
{

Hist2DItem::Hist2DItem( const Math::Regular2DHistogram& hist2D ) :
   m_hist( hist2D ),
   m_pixmap( hist2D.getNumBinsX(), hist2D.getNumBinsY() )
{
   double max = m_hist.getMaximum();
   double min = m_hist.getMinimum();
   double zTot = max - min;

   QPainter p;
   p.begin( &m_pixmap );

   for ( size_t iX = 0; iX < m_hist.getNumBinsX(); ++iX )
   {
      for ( size_t iY = 0; iY < m_hist.getNumBinsY(); ++iY )
      {
         double z = 1 - ( m_hist.getBinContent( iX, iY ) - min ) / zTot;
         /// Due to rounding errors, z can be smaller than zero.
         z = z < 0 ? 0 : z;
         QColor colour;
         colour.setRedF( z );
         colour.setGreenF( z );
         colour.setBlueF( z );
         p.setPen( colour );
         p.drawPoint( QPoint( iX, m_hist.getNumBinsY() - iY - 1 ) );
      }
   }
}

double Hist2DItem::getMinX() const
{
   return m_hist.getMinX();
}

double Hist2DItem::getMaxX() const
{
   return m_hist.getMaxX();
}

double Hist2DItem::getMinY() const
{
   return m_hist.getMinY();
}

double Hist2DItem::getMaxY() const
{
   return m_hist.getMaxY();
}

void Hist2DItem::generatePlotCommands( PaintArea* paintArea ) const
{
   const QRectF& viewport = paintArea->getViewport();
   QRectF histDataRange( getMinX(), getMaxY(), getMaxX() - getMinX(), -( getMaxY() - getMinY() ) );

   PcPixmapPaint* pcPixmapPaint = new PcPixmapPaint( m_pixmap, histDataRange, viewport );
   paintArea->addPaintCommand( pcPixmapPaint );
}

} /// namespace Plotting
