#ifndef PCRECTANGLEPAINT_H
#define PCRECTANGLEPAINT_H

#include "IPaintCommand.h"

#include <QRectF>

class QPointF;

namespace Plotting
{

/// TODO: doc
class PcRectanglePaint : public IPaintCommand
{
   public:
      PcRectanglePaint( const QPointF& topLeft, const QPointF& bottomRight );
      virtual ~PcRectanglePaint();

      void execute( PaintArea& paintArea ) const;

   private:
      QRectF         m_rect;
};

} /// namespace Plotting

#endif // PCRECTANGLEPAINT_H
