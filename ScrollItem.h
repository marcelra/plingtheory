#ifndef SCROLLITEM_H
#define SCROLLITEM_H

#include "IPaintItem.h"

class QRectF;

class ScrollItem : public IPaintItem
{
   public:
      enum Orientation
      {
         Horizontal = 0,
         Vertical
      };
   public:
      ScrollItem( Orientation orientation, double dataMin, double dataMax, double viewMin, double viewMax );

      double getMinX() const;
      double getMaxX() const;
      double getMinY() const;
      double getMaxY() const;

      void setGraphViewPort( const QRectF& viewPort );

   private:
      void generatePlotCommands( PaintArea* paintArea ) const;

   private:
      Orientation    m_orientation;
      double         m_dataMin;
      double         m_dataMax;
      double         m_viewMin;
      double         m_viewMax;


};

#endif // SCROLLITEM_H
