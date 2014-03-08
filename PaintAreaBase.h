#ifndef PAINTAREABASE_H
#define PAINTAREABASE_H

#include <cassert>

#include <QWidget>

class PaintAreaBase : public QWidget
{
   Q_OBJECT

   public:
      PaintAreaBase( QWidget* parent = 0 );
      virtual ~PaintAreaBase();

      QPointF transformToCanvasCoordinates( const QPointF& point ) const;
      QPointF transformToWorldCoordinates( const QPoint& point ) const;

      void setViewPort( const QRectF& viewPort );
      const QRectF& getViewPort() const;

      QPainter& getPainter();
      void setName( const QString& name );

   signals:
      void viewPortChanged( const QRectF& newViewPort );

   protected:
      virtual void paintEvent( QPaintEvent* event );
      virtual void paintEventImpl( QPaintEvent* event ) = 0;

   protected:
      QString     m_name;
      QRectF      m_viewPort;
      QRect       m_canvas;
      QPainter*   m_painter;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods PaintAreaBase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getPainter
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline QPainter& PaintAreaBase::getPainter()
{
   assert( m_painter );
   return *m_painter;
}

#endif // PAINTAREABASE_H
