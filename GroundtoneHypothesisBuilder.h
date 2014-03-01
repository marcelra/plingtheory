#ifndef GROUNDTONEHYPOTHESISBUILDER_H
#define GROUNDTONEHYPOTHESISBUILDER_H

#include "RealVector.h"

namespace WaveAnalysis
{
   class SrSpectrum;
};

namespace FeatureAlgorithm
{

class GroundtoneHypothesisBuilder
{
   public:
      GroundtoneHypothesisBuilder( const WaveAnalysis::SrSpectrum& spec );

      RealVector execute( size_t numIterations );

   private:
      const WaveAnalysis::SrSpectrum&     m_spec;

};

} /// namespace FeatureAlgorithm

#endif // GROUNDTONEHYPOTHESISBUILDER_H
