#ifndef PEAK_H
#define PEAK_H

#include <cstddef>
#include <cassert>
#include <cmath>

namespace Feature
{

/**
 * @class Peak
 * @brief Feature describing a peak in real-valued data series
 */
class Peak
{
   public:
      /**
       * Constructor
       * @param position: index of the array. It is a double type since the peak can be centered between two points in the
       * array
       * @param prominence: prominence of the peak (i.e. height - pedestal at centre)
       * @param width: width at half prominence of the peak
       * @param pedestal: offset
       */
      Peak( double position, double prominence, double width, double pedestal );
      virtual ~Peak();

      /**
       * Get the real-valued position
       */
      double getPosition() const;
      /**
       * Get the position as index
       */
      size_t getPositionIndex() const;
      /**
       * Get the prominence of the peak (@see constructor)
       */
      double getProminence() const;
      /**
       * Get the width
       */
      double getWidth() const;
      /**
       * Get the pedestal
       */
      double getPedestal() const;

      /**
       * Set height
       */
      void setProminence( double prominence );

   private:
      double                m_position;        //! Position of centre of peak
      double                m_prominence;      //! Prominence of peak
      double                m_width;           //! Width at half prominence
      double                m_pedestal;        //! Offset
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double Peak::getPosition() const
{
   return m_position;
}

inline size_t Peak::getPositionIndex() const
{
   assert( m_position > 0 );
   return floor( m_position + 0.5 );
}

inline double Peak::getProminence() const
{
   return m_prominence;
}

inline double Peak::getWidth() const
{
   return m_width;
}

inline double Peak::getPedestal() const
{
   return m_pedestal;
}


} /// namespace Feature

#endif // PEAK_H
