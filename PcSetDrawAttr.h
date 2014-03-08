#ifndef PCSETDRAWATTR_H
#define PCSETDRAWATTR_H

#include "IPaintCommand.h"

#include <QPen>
#include <QBrush>

namespace Plotting
{

class PcSetDrawAttr : public IPaintCommand
{
   public:
      PcSetDrawAttr( QColor lineColor, double lineWidth, bool antialiasing = true );
      PcSetDrawAttr( QColor lineColor, double lineWidth, QColor fillColor, Qt::BrushStyle fillStyle, bool antialiasing = true );
      virtual ~PcSetDrawAttr();

      void execute( PaintArea& paintArea ) const;

   private:
      QPen        m_pen;
      QBrush      m_brush;
      bool        m_antialiasing;
};

} /// namespace Plotting

#endif // PCSETDRAWATTR_H
