#ifndef LINEDRAWATTR_H
#define LINEDRAWATTR_H

#include <QColor>

namespace Plotting
{

class LineDrawAttr
{
   public:
      LineDrawAttr( QColor color = Qt::black, double width = 2, bool antialiasing = true );
      virtual ~LineDrawAttr();

      void setLineColor( const QColor& color );
      const QColor& getLineColor() const;

      void setLineWidth( double width );
      double getLineWidth() const;

      void setAntialiasing( bool antialiasing );
      bool getAntialiasing() const;

   private:
      QColor      m_color;
      double      m_width;
      bool        m_antialiasing;

};

} /// namespace Plotting

#endif // LINEDRAWATTR_H
