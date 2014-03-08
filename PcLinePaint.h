#ifndef PCLINEPAINT_H
#define PCLINEPAINT_H

#include "IPaintCommand.h"

#include <QPointF>

class QPointF;

class PcLinePaint : public IPaintCommand
{
   public:
      PcLinePaint( const QPointF& v0, const QPointF& v1 );
      virtual ~PcLinePaint();

      void execute( PaintArea& paintArea ) const;

   private:
      QPointF     m_v0;
      QPointF     m_v1;

};

#endif // PCLINEPAINT_H
