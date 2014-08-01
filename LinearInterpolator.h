#ifndef LINEARINTERPOLATOR_H
#define LINEARINTERPOLATOR_H

#include "IRealFunction.h"

namespace Math
{

/**
 * @class LinearInterpolator
 * @brief Function that linearly interpolates between points in a data set.
 */
class LinearInterpolator : public IRealFunction
{
   public:
      /**
       * Create linear interpolation between points @param x and @param y. The input arguments will be sorted.
       */
      LinearInterpolator( const RealVector& x, const RealVector& y );

      /**
       * @see IRealFunction
       */
      double operator()( double x ) const;
      /**
       * @see IRealFunction
       */
      IRealFunction* clone() const;

      /**
       * Interpolate between points x0 and x1 at point x (x0 <= x <= x1).
       */
      static double interpolate( double x, double x0, double x1, double y0, double y1 );

   private:

      /**
       * Find the interval in which @param x is contained.
       */
      virtual size_t findInterval( double x ) const;

   private:
      RealVector        m_x;        //! Sorted input.
      RealVector        m_y;        //! Output sorted along with m_x.
      double            m_xWidth;   //! Interval size of m_x.
};

} /// namespace Math

#endif // LINEARINTERPOLATOR_H
