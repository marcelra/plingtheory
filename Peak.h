#ifndef PEAK_H
#define PEAK_H

#include <cstddef>
#include <cassert>
#include <cmath>

namespace Feature
{

/**
 * @class Peak
 * @brief Feature describing a peak in real-valued data series.
 */
class Peak
{
   public:
      /**
       * Constructor
       * @param position: index of the array. It is a double type since the peak can be centered between two points in the
       * array.
       * @param prominence: prominence of the peak (i.e. height - pedestal at centre).
       * @param width: width at half prominence of the peak.
       * @param pedestal: offset.
       */
      Peak( double position, double prominence, double width, double pedestal );
      virtual ~Peak();

      /**
       * Get the real-valued position.
       */
      double getPosition() const;
      /**
       * Get the position as index.
       */
      size_t getPositionIndex() const;
      /**
       * Get the prominence of the peak (@see constructor).
       */
      double getProminence() const;
      /**
       * Get the width.
       */
      double getWidth() const;
      /**
       * Get the pedestal.
       */
      double getPedestal() const;

      /**
       * Set position.
       */
      void setPosition( double posision );
      /**
       * Set height.
       */
      void setProminence( double prominence );
      /**
       * Set width.
       */
      void setWidth( double width );
      /**
       * Set pedestal.
       */
      void setPedestal( double pedestal );

      /**
       * Set links to neighbouring peak.
       */
      void setLeftNeighbourPeak( Peak* peak );
      void setRightNeighbourPeak( Peak* peak );

      /**
       * Get links to neighbouring peaks.
       */
      Peak* getLeftNeighbourPeak();
      Peak* getRightNeighbourPeak();
      const Peak* getLeftNeighbourPeak() const;
      const Peak* getRightNeighbourPeak() const;


   private:
      double                m_position;        //! Position of centre of peak
      double                m_prominence;      //! Prominence of peak
      double                m_width;           //! Width at half prominence
      double                m_pedestal;        //! Offset
      Peak*                 m_leftNeighbour;   //! Pointer to peak left of this peak
      Peak*                 m_rightNeighbour;  //! Pointer to peak right of this peak
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline getter methods.
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Peak::setPosition( double position )
{
   m_position = position;
}

inline void Peak::setWidth( double width )
{
   m_width = width;
}

inline void Peak::setPedestal( double pedestal )
{
   m_pedestal = pedestal;
}

inline void Peak::setProminence( double prominence )
{
   m_prominence = prominence;
}

} /// namespace Feature

#endif // PEAK_H
