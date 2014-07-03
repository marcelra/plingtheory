#ifndef PAINTAREA_H
#define PAINTAREA_H

#include "PaintAreaBase.h"

#include <QPixmap>

#include <memory>

namespace Plotting
{

class GridItem;
class IPaintItem;
class IPaintCommand;

/// TODO: doc
class PaintArea : public PaintAreaBase
{
   Q_OBJECT

   public:
      PaintArea( QWidget* parent = 0 );
      virtual ~PaintArea();

      void addPaintCommand( const IPaintCommand* paintCommand );

      const QRectF& getDataRange() const;

      void wheelEvent( QWheelEvent* event );
      void mouseDoubleClickEvent( QMouseEvent* event );

      void mouseMoveEvent( QMouseEvent* event );
      void mousePressEvent( QMouseEvent* event );
      void mouseReleaseEvent( QMouseEvent* event );
      void keyPressEvent( QKeyEvent* event );

   public:
      void removePaintItem( const IPaintItem* paintItem );
      void addPaintItem( const IPaintItem* paintItem );
      void setGridItem( const GridItem* gridItem );
      void clearAllPaintItems();

      void autoScale();

   private:
      enum ZoomMode
      {
         ZoomBoth = 0,
         ZoomHorizontal,
         ZoomVertical
      };

   private:
      void generatePaintCommands();
      void clearPaintCommands();
      QRectF getZoomViewportHorizontal( double zoomFactor, const QRectF& oldViewport, double xCentre ) const;
      QRectF getZoomViewportVertical( double zoomFactor, const QRectF& oldViewport, double yCentre ) const;

   private:
      std::vector< const IPaintItem* >  m_paintItems;
      const GridItem*                   m_gridItem;

   protected:
      void paintEventImpl( QPaintEvent* event );

      QRectF      m_dataRange;

      std::vector< const IPaintCommand* >  m_paintCommands;

      ZoomMode                 m_zoomMode;

      std::unique_ptr< QPoint >  m_zoomAreaStart;
      std::unique_ptr< QPoint >  m_zoomAreaEnd;
      QPixmap                    m_grabbedWidget;
      std::map< int, QRectF >    m_storedViews;

      static double              s_minSizeAutoScale;
};

} /// namespace Plotting

#endif // PAINTAREA_H
