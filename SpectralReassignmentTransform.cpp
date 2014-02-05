#include "SpectralReassignmentTransform.h"

#include "WindowFuncDef.h"

namespace WaveAnalysis
{

SpectralReassignmentTransform::SpectralReassignmentTransform( const SamplingInfo& samplingInfo, size_t fourierSize, size_t numSamplesZeroPadding, double hopRate ) :
   m_stft( samplingInfo, fourierSize, HanningWindowFuncDef(), numSamplesZeroPadding, hopRate ),
   m_stftDerivative( samplingInfo, fourierSize, HanningDerivativeWindowFuncDef(), numSamplesZeroPadding, hopRate ),
   m_stftTimeRamped( samplingInfo, fourierSize, HanningTimeRampedWindowFuncDef(), numSamplesZeroPadding, hopRate )
{
}

RawStftData::Ptr SpectralReassignmentTransform::execute( const RawPcmData& data )
{
   RawStftData::Ptr stft = m_stft.execute( data );
   RawStftData::Ptr stftDerivative = m_stftDerivative.execute( data );
   RawStftData::Ptr stftTimeRamped = m_stftTimeRamped.execute( data );

   assert( stft->getNumSpectra() == stftDerivative->getNumSpectra() );
   assert( stft->getNumSpectra() == stftTimeRamped->getNumSpectra() );
   RawStftData* result = new RawStftData( m_stft.getConfig() );

   for ( size_t iSpec = 0; iSpec < stft->getNumSpectra(); ++iSpec )
   {
      const FourierSpectrum& ft = stft->getSpectrum( iSpec );
      const FourierSpectrum& ftDerivative = stftDerivative->getSpectrum( iSpec );
      const FourierSpectrum& ftTimeRamped = stftTimeRamped->getSpectrum( iSpec );
      result->addSpectrum( new SRSpectrum( ft, ftDerivative, ftTimeRamped ), new RawStftData::WindowLocation( stft->getWindowLocation( iSpec ) ) );
   }

   return RawStftData::Ptr( result );
}

} /// namespace WaveAnalysis
