#ifndef KERNELPDF_H
#define KERNELPDF_H

#include "IPdf.h"
#include "RealVector.h"

namespace Math
{

class KernelPdf : public IPdf
{
   public:
      KernelPdf( IPdf::CPtr kernel, const RealVector& sampling );
      KernelPdf( IPdf::CPtr kernel, const RealVector& sampling, const RealVector& weights );

   /**
    * IPdf interface (@see IPdf).
    */
   public:
      double getDensity( double x ) const;
      double getProbability( double x0, double x1 ) const;
      double getIntegral( double x ) const;

   private:
      IPdf::CPtr           m_kernel;
      RealVector           m_sampling;
      RealVector           m_weights;
};

} /// namespace Math

#endif // KERNELPDF_H
