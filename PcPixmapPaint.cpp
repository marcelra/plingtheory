#include "PcPixmapPaint.h"

#include "PaintArea.h"

#include <QDebug>
#include <QPainter>

namespace Plotting
{

PcPixmapPaint::PcPixmapPaint( const QPixmap& pixmap, const QRectF& dataRange, const QRectF& viewport ) :
   m_pixmap( pixmap ),
   m_dataRange( dataRange ),
   m_viewport( viewport )
{
}

void PcPixmapPaint::execute( PaintArea& paintArea ) const
{
   const QRectF& visiblePart = m_viewport.intersected( m_dataRange );
   QRect canvas = paintArea.getCanvas();

   double targetXMin = ( visiblePart.left() - m_viewport.left() ) / m_viewport.width() * canvas.width();
   double targetXMax = ( visiblePart.right() - m_viewport.left() ) / m_viewport.width() * canvas.width();
   double targetYMin = ( visiblePart.top() - m_viewport.top() ) /  m_viewport.height() * canvas.height();
   double targetYMax = ( visiblePart.bottom() - m_viewport.top() ) / m_viewport.height() * canvas.height();

   QRectF target( canvas );
   target.setLeft( targetXMin );
   target.setRight( targetXMax );
   target.setBottom( targetYMin );
   target.setTop( targetYMax );

   double sourceXMin = ( visiblePart.left() - m_dataRange.left() ) / m_dataRange.width() * m_pixmap.width();
   double sourceXMax = ( visiblePart.right() - m_dataRange.left() ) / m_dataRange.width() * m_pixmap.width();
   double sourceYMin = ( visiblePart.top() - m_dataRange.top() ) /  m_dataRange.height() * m_pixmap.height();
   double sourceYMax = ( visiblePart.bottom() - m_dataRange.top() ) / m_dataRange.height() * m_pixmap.height();

   QRectF source( QPoint( 0, 0 ), m_pixmap.size() );
   source.setLeft( sourceXMin );
   source.setRight( sourceXMax );
   source.setBottom( sourceYMin );
   source.setTop( sourceYMax );

   QPainter& painter = paintArea.getPainter();
   painter.drawPixmap( target, m_pixmap, source );
}

} /// namespace Plotting
