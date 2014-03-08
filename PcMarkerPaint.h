#ifndef PCMARKERPAINT_H
#define PCMARKERPAINT_H

#include "IPaintCommand.h"

#include <QPointF>

namespace Plotting
{

class PcMarkerPaint : public IPaintCommand
{
   public:
      PcMarkerPaint( const QPointF& location, double markerSize );
      virtual ~PcMarkerPaint();

      void execute( PaintArea& paintArea ) const;

   private:
      QPointF   m_location;
      double    m_markerSize;
};

} /// namespace Plotting

#endif // PCMARKERPAINT_H
