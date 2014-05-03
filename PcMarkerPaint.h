#ifndef PCMARKERPAINT_H
#define PCMARKERPAINT_H

#include "IPaintCommand.h"
#include "MarkerDrawAttr.h"

#include <QPointF>

namespace Plotting
{

/// TODO: doc
class PcMarkerPaint : public IPaintCommand
{
   public:
      /// TODO: use MarkerDrawAttr here?
      PcMarkerPaint( const QPointF& location, double markerSize, MarkerType markerType );
      virtual ~PcMarkerPaint();

      void execute( PaintArea& paintArea ) const;

   private:
      QPointF     m_location;
      double      m_markerSize;
      MarkerType  m_markerType;
};

} /// namespace Plotting

#endif // PCMARKERPAINT_H
