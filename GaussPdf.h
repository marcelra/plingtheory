#ifndef GAUSSPDF_H
#define GAUSSPDF_H

#include "IPdf.h"

namespace Math
{

/**
 * @class GaussPdf
 * @brief Class that describes an untruncated Gaussian PDF.
 */
class GaussPdf : public IPdf
{
   public:
      /**
       * Constructor. Create a Gauss PDF with parameters @param mu and @param sigma.
       */
      GaussPdf( double mu, double sigma );

   /**
    * IPdf interface (@see IPdf).
    */
   public:
      double getDensity( double x ) const;
      double getProbability( double x0, double x1 ) const;
      double getIntegral( double x ) const;

   public:
      /**
       * Get parameter mu.
       */
      double getMu() const;
      /**
       * Get parameter sigma.
       */
      double getSigma() const;


   private:
      double      m_mu;
      double      m_sigma;

   /**
    * Derived quantities that speed up the calculation.
    * m_norm * exp( m_argFac * ( x - mu )^2 )
    */
   private:
      double      m_gaussArgFac;
      double      m_erfArgFac;
      double      m_norm;
};

} /// namespace Math

#endif // GAUSSPDF_H
