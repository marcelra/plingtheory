#include "AxisPaintArea.h"

#include <QPainter>
#include <QPaintEvent>

#include <QDebug>

#include <cmath>

AxisPaintArea::AxisPaintArea( QWidget* parent ) :
   PaintAreaBase( parent )
{
   m_minDistMajTicks = 100;
}

AxisPaintArea::~AxisPaintArea()
{}

void AxisPaintArea::paintEventImpl( QPaintEvent* paintEvent )
{
   assert( paintEvent );

   drawAxisLine();

   updateTicks();

   for ( size_t iTick = 0; iTick < m_majorTicks.size(); ++iTick )
   {
      drawMajorTick( m_majorTicks[ iTick ] );
   }
   for ( size_t iMinorTick = 0; iMinorTick < m_minorTicks.size(); ++iMinorTick )
   {
      drawMinorTick( m_minorTicks[ iMinorTick ] );
   }
}

void AxisPaintArea::updateTicks()
{
   double min = getMin();
   double max = getMax();

   size_t numTicksMax = getNumsPixelsAlongAxis() / m_minDistMajTicks;
   double scale = max - min;

   double unroundedDist = scale / numTicksMax;

   /// TODO: can be done more efficient.
   int nTries = 1000;
   int powTen = 0;
   while ( unroundedDist > 20 && nTries >= 0 )
   {
      unroundedDist /= 10;
      ++powTen;
      --nTries;
   }
   while ( unroundedDist < 2 && nTries >= 0 )
   {
      unroundedDist *= 10;
      --powTen;
      --nTries;
   }
   if ( nTries <= 0 )
   {
      qDebug() << "Failed to create axis. Probably zoom level is too large or too small.";
      return;
   }

   assert( unroundedDist > 2 && unroundedDist < 20 );

   size_t numMinorTicks;
   double tickBase;
   if ( unroundedDist < 5 )
   {
      tickBase = 2;
      numMinorTicks = 4;
   }
   else if ( unroundedDist < 10 )
   {
      tickBase = 5;
      numMinorTicks = 5;
   }
   else
   {
      tickBase = 10;
      numMinorTicks = 5;
   }

   m_tickBase = tickBase;

   tickBase *= pow( 10, powTen );

   double firstTick = ceil( min / tickBase ) * tickBase;
   double lastTick = ceil( max / tickBase ) * tickBase;

   m_majorTicks.clear();
   int majorTicksMax = 1000;
   double currTick = firstTick;
   while ( currTick < lastTick && --majorTicksMax >=0 )
   {
      m_majorTicks.push_back( currTick );
      currTick += tickBase;
   }
   if ( majorTicksMax == 0 )
   {
      m_majorTicks.clear();
      return;
   }

   int minorTicksMax = 1000;
   m_minorTicks.clear();
   if ( m_majorTicks.size() > 0 )
   {
      double firstMajorTick = m_majorTicks[0];
      double minorTickStep = tickBase / numMinorTicks;
      double currMinorTick = firstMajorTick;
      while ( currMinorTick > min && --minorTicksMax >= 0 )
      {
         m_minorTicks.push_back( currMinorTick );
         currMinorTick -= minorTickStep;
      }
      currMinorTick = firstMajorTick;
      while ( currMinorTick < max && --minorTicksMax >= 0 )
      {
         m_minorTicks.push_back( currMinorTick );
         currMinorTick += minorTickStep;
      }
      if ( minorTicksMax == 0 )
      {
         m_minorTicks.clear();
         return;
      }
   }

}

const std::vector< double >& AxisPaintArea::getMajorTicks() const
{
   return m_majorTicks;
}
