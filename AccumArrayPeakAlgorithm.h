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

      std::vector< Feature::Peak > execute( const Math::RegularAccumArray& data ) const;

      void setDoMonitor( bool doMonitor );

   private:
      RealVector calculateSmoothedData( const Math::RegularAccumArray& data ) const;
      RealVector subtractBaseline( const RealVector& smoothedData, const RealVector& originalData ) const;

      std::vector< Feature::Peak > findPeaks( const RealVector& baselineSubtractedData, const Math::RegularAccumArray& data ) const;
      void dressPeaks( const Math::RegularAccumArray& data, const RealVector& baselineSubtractedData, std::vector< Feature::Peak >& peaks ) const;

   private:
      double         m_smoothFraction;
      double         m_sigmaFactor;
      bool           m_doMonitor;
      size_t         m_maxNumPeaks;
};

} /// FeatureAlgorithm

#endif // PEAKALGORITHM_H
