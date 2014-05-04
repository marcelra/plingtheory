#ifndef PCPIXMAPPAINT_H
#define PCPIXMAPPAINT_H

#include "IPaintCommand.h"

#include <QPixmap>

namespace Plotting
{

class PcPixmapPaint : public IPaintCommand
{
   public:
      PcPixmapPaint( const QPixmap& pixmap, const QRectF& dataRange, const QRectF& viewport );

      void execute( PaintArea& paintArea ) const;

   private:
      const QPixmap&   m_pixmap;
      QRectF           m_dataRange;
      QRectF           m_viewport;
};

} /// namespace Plotting

#endif // PCPIXMAPPAINT_H
