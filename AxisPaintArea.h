#ifndef AXISPAINTAREA_H
#define AXISPAINTAREA_H

#include "PaintAreaBase.h"

namespace Plotting
{

class AxisPaintArea : public PaintAreaBase
{
   Q_OBJECT

   public:
      AxisPaintArea( QWidget* parent = 0 );
      virtual ~AxisPaintArea();

      virtual void paintEventImpl( QPaintEvent* event );

      const std::vector< double >& getMajorTicks() const;

   public:
      virtual double getMin() const = 0;
      virtual double getMax() const = 0;

   protected:
      virtual void drawAxisLine() = 0;
      virtual void drawMajorTick( double tickValue ) = 0;
      virtual void drawMinorTick( double tickValue ) = 0;
      virtual int getNumsPixelsAlongAxis() = 0;

   private:
      void updateTicks();

   protected:

      size_t      m_minDistMajTicks;
      size_t      m_maxDistMajTicks;

      size_t                     m_tickBase;
      std::vector< double >      m_majorTicks;
      std::vector< double >      m_minorTicks;
};

} /// namespace Plotting

#endif // AXISPAINTAREA_H
