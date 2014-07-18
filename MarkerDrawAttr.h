#ifndef MARKERDRAWATTR_H
#define MARKERDRAWATTR_H

#include "LineDrawAttr.h"

namespace Plotting
{

enum MarkerType
{
   MarkerRectangle = 0,
   MarkerCircle,
   MarkerCross,
   MarkerPlus
};

class MarkerDrawAttr : public LineDrawAttr
{
   public:
      MarkerDrawAttr( QColor color = Qt::black, MarkerType markerType = MarkerPlus, double markerSize = 3, double width = 2, bool antialiasing = true );

      void setMarkerType( Plotting::MarkerType markerType );
      void setMarkerSize( double markerSize );

      MarkerType getMarkerType() const;
      double getMarkerSize() const;

   private:
      MarkerType     m_markerType;
      double         m_markerSize;
};

} /// namespace Plotting

#endif // MARKERDRAWATTR_H
