#ifndef GROUNDTONEHYPOTHESISBUILDER_H
#define GROUNDTONEHYPOTHESISBUILDER_H

#include "RealVector.h"

namespace WaveAnalysis
{
   class SRSpectrum;
};

namespace FeatureAlgorithm
{

class GroundtoneHypothesisBuilder
{
   public:
      GroundtoneHypothesisBuilder( const WaveAnalysis::SRSpectrum& spec );

      RealVector execute( size_t numIterations );

   private:
      const WaveAnalysis::SRSpectrum&     m_spec;

};

} /// namespace FeatureAlgorithm

#endif // GROUNDTONEHYPOTHESISBUILDER_H
