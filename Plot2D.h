#ifndef PLOT2D_H
#define PLOT2D_H

#include <QWidget>
#include <QMetaType>

class QGridLayout;
class QHBoxLayout;
class QScrollBar;
class QVBoxLayout;
class QWheelEvent;

namespace Plotting
{

class CurveItem;
class GridItem;
class IPaintItem;
class PaintArea;
class ScrollItem;
class ScrollPaintArea;
class XAxisPaintArea;
class YAxisPaintArea;

class Plot2D : public QWidget
{
   Q_OBJECT

   public:
      Plot2D( QWidget* parent = 0 );
      virtual ~Plot2D();

      void addItem( const IPaintItem* item );
      void setViewPort( const QRectF& viewPort );
      void setEnableGrid( bool enableGrid );

      // void wheelEvent( QWheelEvent* event );
      // void mouseDoubleClickEvent( QMouseEvent* event );

   private slots:
      void synchroniseViewPorts( const QRectF& viewPort );

   private:
      QHBoxLayout*       m_horizontalLayout;
      QVBoxLayout*       m_verticalLayoutYScroll;
      QVBoxLayout*       m_verticalLayoutCanvas;
      QGridLayout*       m_gridLayout;
      XAxisPaintArea*    m_xAxisPaintArea;
      YAxisPaintArea*    m_yAxisPaintArea;
      PaintArea*         m_graph;
      PaintArea*         m_empty;
      ScrollPaintArea*   m_xScroll;
      ScrollPaintArea*   m_yScroll;

      ScrollItem*        m_xScrollItem;
      ScrollItem*        m_yScrollItem;
      GridItem*          m_gridItem;

      bool               m_horizontalMouseWheel;
};

} /// namespace Plotting

Q_DECLARE_METATYPE( Plotting::Plot2D* );

#endif // PLOT2D_H
