#include "GroundtoneHypothesisBuilder.h"

#include "RegularAccumArray.h"
#include "SrSpectrum.h"

/// TODO: temp
#include "TCanvas.h"
#include "TH1F.h"

namespace FeatureAlgorithm
{

GroundtoneHypothesisBuilder::GroundtoneHypothesisBuilder( const WaveAnalysis::SrSpectrum& spec ) :
   m_spec( spec )
{}

RealVector GroundtoneHypothesisBuilder::execute( size_t numIterations )
{
   RealVector result;

   Math::RegularAccumArray hist = m_spec.rebinToFourierLattice();

   const RealVector& magnitudes = m_spec.getMagnitude();
   for ( size_t iIter = 1; iIter < numIterations; ++iIter )
   {
      double fac = 1. / ( iIter + 1 );
      const RealVector& frequencies = m_spec.getFrequencies();
      for ( size_t iFreq = 0; iFreq < frequencies.size(); ++iFreq )
      {
         hist.add( frequencies[ iFreq ] * fac, magnitudes[ iFreq ] );
      }
   }

   return result;
}

} /// namespace FeatureAlgorithm
