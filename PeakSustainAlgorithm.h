#ifndef PEAKSUSTAINALGORITHM_H
#define PEAKSUSTAINALGORITHM_H

#include "AlgorithmBase.h"
#include "IBasicSpectrumPeak.h"

#include <vector>

namespace Feature
{

class SustainedPeak : IBasicSpectrumPeak
{
   public:
      SustainedPeak( const IBasicSpectrumPeak* firstPeak );

      void connectPeak( const IBasicSpectrumPeak* nextPeak );
      void finishBuilding();

      const std::vector< const IBasicSpectrumPeak* >& getAllPeaks() const;

   /**
    * IBasicSpectrumPeak interface.
    */
   public:

      /**
       * Returns the mean frequency of all the peaks that are connected.
       */
      double getFrequency() const;
      /**
       * Returns the mean height of all the peaks that are connected.
       */
      double getHeight() const;
      /**
       * Returns zero for the moment (TODO).
       */
      double getFrequencyUncertainty() const;

      /**
       * Returns the start of the first connected peak in samples.
       */
      size_t getStartTimeSamples() const;
      /**
       * Returns the end of the last connected peak in samples.
       */
      size_t getEndTimeSamples() const;

   private:
      std::vector< const IBasicSpectrumPeak* >  m_connectedPeaks;
      double                                    m_meanFrequency;
      double                                    m_meanHeight;
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
