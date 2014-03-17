#ifndef PEAK_H
#define PEAK_H

#include "RealVector.h"

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
      Peak( size_t positionIndex );
      // Peak( double position, double prominence, double width, double pedestal );
      virtual ~Peak();

      /**
       * Get the real-valued position.
       */
      double getPosition() const;
      /**
       * Get the position index in data set.
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
       * Set left and right bound indices.
       */
      void setBoundIndices( size_t leftIndex, size_t rightIndex );
      /**
       * Set bounds in x-axis coordinates.
       */
      void setBounds( double xLeft, double xRight );

      /**
       * Set peak data.
       */
      void setData( const RealVector& data );
      /**
       * Get peak data.
       */
      const RealVector& getData() const;

      /**
       * Get left and right bound indices.
       */
      size_t getLeftBoundIndex() const;
      size_t getRightBoundIndex() const;
      double getLeftBound() const;
      double getRightBound() const;

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
      size_t                m_positionIndex;   //! Position of peak in data set
      size_t                m_leftBoundIndex;  //! Index of left bound
      size_t                m_rightBoundIndex; //! Index of right bound
      double                m_leftBound;       //! Left bound
      double                m_rightBound;      //! Right bound
      double                m_position;        //! Position of centre of peak
      double                m_prominence;      //! Prominence of peak
      double                m_width;           //! Width at half prominence
      double                m_pedestal;        //! Offset
      Peak*                 m_leftNeighbour;   //! Pointer to peak left of this peak
      Peak*                 m_rightNeighbour;  //! Pointer to peak right of this peak
      RealVector            m_data;            //! Data points between peak minima
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
   return m_positionIndex;
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

inline double Peak::getLeftBound() const
{
   return m_leftBound;
}

inline double Peak::getRightBound() const
{
   return m_rightBound;
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
