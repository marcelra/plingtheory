#ifndef PLOT2D_H
#define PLOT2D_H

#include <QWidget>
#include <QMetaType>

/// Forward declares
class QGridLayout;

namespace Plotting
{

/// Forward declares in Plotting namespace
class CurveItem;
class GridItem;
class IPaintItem;
class PaintArea;
class ScrollPaintArea;
class AxisPaintArea;

/// TODO: doc
class Plot2D : public QWidget
{
   Q_OBJECT

   public:
      Plot2D( QWidget* parent = 0 );
      virtual ~Plot2D();

      void addItem( const IPaintItem* item );
      void setEnableGrid( bool enableGrid );
      void setViewportConstraintsToData();

   public slots:
      void setViewport( const QRectF& viewport );

   private slots:
      void synchroniseViewports( const QRectF& viewport );
      void synchroniseWithGraphViewport();

   private:
      QGridLayout*       m_gridLayout;

   private:
      AxisPaintArea*     m_xAxisPaintArea;
      AxisPaintArea*     m_yAxisPaintArea;
      PaintArea*         m_graph;
      PaintArea*         m_empty;
      ScrollPaintArea*   m_xScroll;
      ScrollPaintArea*   m_yScroll;

   private:
      GridItem*          m_gridItem;
};

} /// namespace Plotting

Q_DECLARE_METATYPE( Plotting::Plot2D* )

#endif // PLOT2D_H
