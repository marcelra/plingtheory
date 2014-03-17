#ifndef PAINTAREA_H
#define PAINTAREA_H

#include "PaintAreaBase.h"

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

   public:
      void removePaintItem( const IPaintItem* paintItem );
      void addPaintItem( const IPaintItem* paintItem );
      void setGridItem( const GridItem* gridItem );
      void clearAllPaintItems();

      void autoScale();

    private:
      void generatePaintCommands();
      void clearPaintCommands();

   private:
      std::vector< const IPaintItem* >  m_paintItems;
      const IPaintItem*                 m_gridItem;

   protected:
      void paintEventImpl( QPaintEvent* event );

      QRectF      m_dataRange;

      std::vector< const IPaintCommand* >  m_paintCommands;

      bool        m_horizontalMouseWheel;

};

} /// namespace Plotting

#endif // PAINTAREA_H
