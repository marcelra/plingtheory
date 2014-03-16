#ifndef UNIFORMPDF_H
#define UNIFORMPDF_H

#include "IPdf.h"

namespace Math
{

/**
 * @class UniformPdf
 * @brief Describes a uniform probability distribution function.
 */
class UniformPdf : public IPdf
{
   public:
      UniformPdf( double min, double max );

   /**
    * Interface implementation of IPdf (@see IPdf).
    */
   public:
      double getDensity( double x ) const;
      double getProbability( double x0, double x1 ) const;
      double getIntegral( double x ) const;

   public:
      double getMinX() const;
      double getMaxX() const;

   private:
      double      m_min;               //! Min of the distribution.
      double      m_max;               //! Max of the distribution.
      double      m_density;           //! Density.
};

} /// namespace Math

#endif // UNIFORMPDF_H
