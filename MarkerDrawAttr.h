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
      MarkerDrawAttr( QColor color = Qt::black, MarkerType markerType = MarkerRectangle, double width = 2, bool antialiasing = false );

      void setMarkerType( Plotting::MarkerType markerType );
      MarkerType getMarkerType() const;

   private:
      MarkerType     m_markerType;
};

} /// namespace Plotting

#endif // MARKERDRAWATTR_H
