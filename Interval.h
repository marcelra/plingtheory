#ifndef INTERVAL_H
#define INTERVAL_H

/**
 * @class Interval.
 * @brief Describes an interval.
 */
class Interval
{
   public:
      /**
       * Create an interval from @param min to @param max. It is asserted that @param min is smaller or equal to @param max.
       */
      Interval( double min, double max );

      /**
       * Set the minimum/maximum value.
       */
      void setMin( double min );
      void setMax( double max );

      /**
       * Get the minimum/maximum value.
       */
      double getMin() const;
      double getMax() const;
      /**
       * Get the length of the interval.
       */
      double getLength() const;

      /**
       * Creates an interval from @param value1 and @param value2. The values do not have to be sorted.
       */
      static Interval createFromValues( double value1, double value2 );

      /**
       * Returns true if @param x is within the interval.
       */
      bool contains( double x ) const;

   private:
      double      m_min;         //! Minimum value.
      double      m_max;         //! Maximum value.
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods Interval
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setMin
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Interval::setMin( double min )
{
   m_min = min;
}

inline void Interval::setMax( double max )
{
   m_max = max;
}

inline double Interval::getMin() const
{
   return m_min;
}

inline double Interval::getMax() const
{
   return m_max;
}

inline double Interval::getLength() const
{
   return m_max - m_min;
}

#endif // RANGE_H
