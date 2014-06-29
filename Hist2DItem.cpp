#include "Hist2DItem.h"

#include "PaintArea.h"
#include "Palette.h"
#include "PcPixmapPaint.h"
#include "Regular2DHistogram.h"

#include <QColor>
#include <QImage>
#include <QPainter>
#include <QDebug>

#include <cmath>

namespace Plotting
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Hist2DItem::Hist2DItem( const Math::Regular2DHistogram& hist2D, const Palette& palette ) :
   m_relPixmap( hist2D.getNumBinsX(), std::vector< double >( hist2D.getNumBinsY() ) ),
   m_minX( hist2D.getMinX() ),
   m_maxX( hist2D.getMaxX() ),
   m_minY( hist2D.getMinY() ),
   m_maxY( hist2D.getMaxY() ),
   m_palette( palette )
{
   assert( hist2D.getNumBinsX() > 0 && hist2D.getNumBinsY() > 0 );

   double max = hist2D.getMaximum();
   double min = hist2D.getMinimum();
   double zTot = ( max - min );

   for ( size_t iX = 0; iX < hist2D.getNumBinsX(); ++iX )
   {
      for ( size_t iY = 0; iY < hist2D.getNumBinsY(); ++iY )
      {
         double z = ( hist2D.getBinContent( iX, iY ) - min ) / zTot;
         m_relPixmap[ iX ][ hist2D.getNumBinsY() - iY - 1 ] = z;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMinX
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Hist2DItem::getMinX() const
{
   return m_minX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMaxX
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Hist2DItem::getMaxX() const
{
   return m_maxX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMinY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Hist2DItem::getMinY() const
{
   return m_minY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMaxY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Hist2DItem::getMaxY() const
{
   return m_maxY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getPixmap
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const QPixmap& Hist2DItem::getPixmap( double compressionX, double compressionY ) const
{
   size_t xCompIndex = compressionX > 1 ? ceil( log( compressionX ) / log( 2 ) ) : 0;
   size_t yCompIndex = compressionY > 1 ? ceil( log( compressionY ) / log( 2 ) ) : 0;

   QPixmap& pixmap = m_cache[ xCompIndex ][ yCompIndex ];
   if ( pixmap.size() == QSize( 0, 0 ) )
   {
      qDebug() << "Creating cached pixmap.";
      fillPixmap( pixmap, xCompIndex, yCompIndex );
   }

   return pixmap;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// fillPixmap
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Hist2DItem::fillPixmap( QPixmap& pixmap, size_t xCompIndex, size_t yCompIndex ) const
{
   size_t xCompression = pow( 2, xCompIndex );
   size_t yCompression = pow( 2, yCompIndex );

   size_t xSizeSource = m_relPixmap.size();
   size_t ySizeSource = m_relPixmap[ 0 ].size();

   QSize newPixmapSize;
   newPixmapSize.setWidth( xSizeSource / xCompression );
   newPixmapSize.setHeight( ySizeSource / yCompression );

   double posX = 0;
   double posXStep = static_cast< double >( xSizeSource ) / newPixmapSize.width();
   double posY = 0;
   double posYStep = static_cast< double >( ySizeSource ) / newPixmapSize.height();

   QImage image( newPixmapSize, QImage::Format_ARGB32 );
   QRgb* bits = reinterpret_cast< QRgb* >( image.bits() );

   size_t iBit = 0;

   for ( int iY = 0; iY < newPixmapSize.height(); ++iY, posY += posYStep )
   {
      posX = 0;
      for ( int iX = 0; iX < newPixmapSize.width(); ++iX, posX += posXStep, ++iBit )
      {
         bits[ iBit ] = getMaxColourInRect( QPoint( posX, posY ), QSize( xCompression, yCompression ) );
      }
   }

   pixmap = QPixmap::fromImage( image );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMaxColourInRect
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QRgb Hist2DItem::getMaxColourInRect( const QPoint& pos, const QSize& rect ) const
{
   double max = 0;
   for ( size_t iX = pos.x(); ( (int ) iX < pos.x() + rect.width() ) && iX < m_relPixmap.size(); ++iX )
   {
      for ( size_t iY = pos.y(); ( ( int ) iY < pos.y() + rect.height() ) && iY < m_relPixmap[ iX ].size(); ++iY )
      {
         double val = m_relPixmap[ iX ][ iY ];
         if ( val > max )
         {
            max = val;
         }
      }
   }
   return m_palette.getRgb( max );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// generatePlotCommands
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Hist2DItem::generatePlotCommands( PaintArea* paintArea ) const
{
   const QRectF& viewport = paintArea->getViewport();

   QRectF histDataRange( getMinX(), getMaxY(), getMaxX() - getMinX(), -( getMaxY() - getMinY() ) );

   double numPixelsX = viewport.width() / histDataRange.width() * m_relPixmap.size();
   double numPixelsY = viewport.height() / histDataRange.height() * m_relPixmap[ 0 ].size();

   double compressionX = numPixelsX / paintArea->getCanvas().width();
   double compressionY = numPixelsY / paintArea->getCanvas().height();

   const QPixmap& pixmap = getPixmap( compressionX, compressionY );

   PcPixmapPaint* pcPixmapPaint = new PcPixmapPaint( pixmap, histDataRange, viewport );
   paintArea->addPaintCommand( pcPixmapPaint );
}

} /// namespace Plotting
