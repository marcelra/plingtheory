#ifndef ACCUMARRAYPEAKALGORITHM_H
#define ACCUMARRAYPEAKALGORITHM_H

#include "Peak.h"
#include "RegularAccumArray.h"

#include <vector>

namespace FeatureAlgorithm
{

class AccumArrayPeakAlgorithm
{
   public:
      AccumArrayPeakAlgorithm();

      std::vector< Feature::Peak > execute( const Math::RegularAccumArray& data );

   private:
      RealVector calculateSmoothedData( const Math::RegularAccumArray& data ) const;
      RealVector subtractBaseline( const RealVector& smoothedData, const RealVector& originalData ) const;

   private:
      double         m_smoothFraction;
      double         m_sigmaFactor;

};

} /// FeatureAlgorithm

#endif // PEAKALGORITHM_H
