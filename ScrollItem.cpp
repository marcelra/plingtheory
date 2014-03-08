#include "ScrollItem.h"

#include "PaintArea.h"
#include "PcRectanglePaint.h"

#include <QDebug>
#include <QPen>
#include <QRectF>

namespace Plotting
{

ScrollItem::ScrollItem( Orientation orientation, double dataMin, double dataMax, double viewMin, double viewMax ) :
   m_orientation( orientation ),
   m_dataMin( dataMin ),
   m_dataMax( dataMax ),
   m_viewMin( viewMin ),
   m_viewMax( viewMax )
{
   qDebug() << "ScrollItem constructor with instance " << this;
}

double ScrollItem::getMinX() const
{
   if ( m_orientation == Horizontal )
   {
      return m_dataMin;
   }
   else
   {
      return 0;
   }
}

double ScrollItem::getMaxX() const
{
   if ( m_orientation == Horizontal )
   {
      return m_dataMax;
   }
   else
   {
      return 1;
   }
}

double ScrollItem::getMinY() const
{
   if ( m_orientation == Vertical )
   {
      return m_dataMin;
   }
   else
   {
      return 0;
   }
}

double ScrollItem::getMaxY() const
{
   if ( m_orientation == Vertical )
   {
      return m_dataMax;
   }
   else
   {
      return 1;
   }
}

void ScrollItem::setGraphViewPort( const QRectF& viewPort )
{
   if ( m_orientation == Horizontal )
   {
      m_viewMin = viewPort.x();
      m_viewMax = viewPort.x() + viewPort.width();
   }
   else
   {
      m_viewMin = viewPort.y();
      m_viewMax = viewPort.y() + viewPort.height();
   }
}

void ScrollItem::generatePlotCommands( PaintArea* paintArea ) const
{
   if ( m_orientation == Horizontal )
   {
      PcRectanglePaint* rectPaintCmd = new PcRectanglePaint( QPointF( m_dataMin, 0), QPointF( m_dataMax, 1 ) );
      paintArea->addPaintCommand( rectPaintCmd );
      // paintArea->setViewPort( m_dataMin, 0, m_dataMax, 1 );
      /// TODO: make PcRectanglePaint
      // paintArea->addRectangle( m_dataMin, 0, m_dataMax, 1 );
      // paintArea->addRectangle( m_viewMin, 0, m_viewMax, 1 );
   }
   else
   {
      PcRectanglePaint* rectPaintCmd = new PcRectanglePaint( QPointF( 0, m_dataMin ), QPointF( 1, m_dataMax ) );
      paintArea->addPaintCommand( rectPaintCmd );
      // paintArea->setViewPort( 0, m_dataMin, 1, m_dataMax );
      // paintArea->addRectangle( 0, m_dataMin, 1, m_dataMax );
      // paintArea->addRectangle( 0, m_viewMin, 1, m_viewMax );
    }
}

} /// namespace Plotting
