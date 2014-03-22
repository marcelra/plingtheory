#ifndef INTERVAL_H
#define INTERVAL_H

class Interval
{
   public:
      Interval( double min, double max );

      void setMin( double min );
      void setMax( double max );

      double getMin() const;
      double getMax() const;
      double getLength() const;

      /**
       * Creates an interval.
       */
      static Interval createFromValues( double value1, double value2 );

      bool contains( double x ) const;

   private:
      double      m_min;
      double      m_max;
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
