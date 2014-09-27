#ifndef AXISPAINTAREA_H
#define AXISPAINTAREA_H

#include "PaintAreaBase.h"

namespace Plotting
{

/**
 * @class AxisPaintArea
 * @brief Abstract class for linear axis display.
 */
class AxisPaintArea : public PaintAreaBase
{
   Q_OBJECT

   public:
      /**
       * Constructor.
       */
      AxisPaintArea( QWidget* parent = 0 );

      /**
       * Handle paint events, i.e. paint the axis.
       */
      virtual void paintEventImpl( QPaintEvent* event );

      /**
       * Query the major ticks.
       */
      const std::vector< double >& getMajorTicks() const;
      /**
       * Set the axis title.
       */
      void setTitle( const std::string& title );
      /**
       * Get the axis title.
       */
      const std::string& getTitle() const;


   public:
      /**
       * This function should calculate the lowest value displayed on the axis.
       */
      virtual double getMin() const = 0;
      /**
       * This function should calculate the highest value displayed on the axis.
       */
      virtual double getMax() const = 0;

   protected:
      /**
       * This method should draw an axis line.
       */
      virtual void drawAxisLine() = 0;
      /**
       * This method should draw the major ticks given a tick value @param tickValue.
       */
      virtual void drawMajorTick( double tickValue ) = 0;
      /**
       * This method should draw the minor ticks given a tick value @param tickValue.
       */
      virtual void drawMinorTick( double tickValue ) = 0;
      /**
       * This method should return the number of pixels along the axis.
       */
      virtual int getNumsPixelsAlongAxis() = 0;
      /**
       * Draws the axis title.
       */
      virtual void drawAxisTitle() = 0;
      /**
       * drawKiloPower
       */
      virtual void drawKiloPower() = 0;

   private:
      /**
       * This method will update the members m_majorTicks and m_minorTicks.
       */
      virtual void updateTicks();

   protected:
      size_t      m_minDistMajTicks;               //! The min dist in pixels between two major ticks.
      double      m_kiloPower;                     //! The factor of 10^3 for each axis label.

      size_t                     m_tickBase;       //! Stores the tickbase, i.e. ( 2, 5, 10 ).
      std::vector< double >      m_majorTicks;     //! Stores the major ticks. Modified by updateTicks.
      std::vector< double >      m_minorTicks;     //! Stores the major ticks. Modified by updateTicks.
      std::string                m_title;          //! The axis title.
};

} /// namespace Plotting

#endif // AXISPAINTAREA_H
