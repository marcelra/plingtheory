#ifndef PEAKSUSTAINALGORITHM_H
#define PEAKSUSTAINALGORITHM_H

#include "AlgorithmBase.h"
#include "IBasicSpectrumPeak.h"

#include <vector>

namespace Feature
{

class SustainedPeak
{
   public:
      SustainedPeak( const IBasicSpectrumPeak* firstPeak );

      void connectPeak( const IBasicSpectrumPeak* nextPeak );

      const std::vector< const IBasicSpectrumPeak* >& getAllPeaks() const;

      size_t getStartTimeSamples() const;
      size_t getEndTimeSamples() const;

   private:
      std::vector< const IBasicSpectrumPeak* >  m_connectedPeaks;
};

} /// namespace Feature

namespace FeatureAlgorithm
{

class PeakSustainAlgorithm : public AlgorithmBase
{
   public:
      PeakSustainAlgorithm( const std::string& name = "PeakSustainAlgorithm", const AlgorithmBase* parent = 0 );

      std::vector< Feature::SustainedPeak* > execute( const std::vector< std::vector< Feature::IBasicSpectrumPeak* > >& peaks );

};

} /// namespace FeatureAlgorithm


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods SustainedPeak
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Feature
{

inline const std::vector< const IBasicSpectrumPeak* >& SustainedPeak::getAllPeaks() const
{
   return m_connectedPeaks;
}

} /// namespace Feature

#endif // PEAKSUSTAINALGORITHM_H
