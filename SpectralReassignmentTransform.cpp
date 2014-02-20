#include "SpectralReassignmentTransform.h"

#include "Logger.h"

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
   Logger msg( "SpectralReassignmentTransform" );
   msg << Msg::Verbose << "In execute..." << Msg::EndReq;

   msg << Msg::Verbose << "Executing the STFT transforms..." << Msg::EndReq;
   RawStftData::Ptr stft = m_stft.execute( data );
   RawStftData::Ptr stftDerivative = m_stftDerivative.execute( data );
   RawStftData::Ptr stftTimeRamped = m_stftTimeRamped.execute( data );

   assert( stft->getNumSpectra() == stftDerivative->getNumSpectra() );
   assert( stft->getNumSpectra() == stftTimeRamped->getNumSpectra() );
   RawStftData* result = new RawStftData( m_stft.getConfig() );

   msg << Msg::Verbose << "Calculating the reassigned spectra." << Msg::EndReq;

   for ( size_t iSpec = 0; iSpec < stft->getNumSpectra(); ++iSpec )
   {
      const FourierSpectrum& ft = stft->getSpectrum( iSpec );
      const FourierSpectrum& ftDerivative = stftDerivative->getSpectrum( iSpec );
      const FourierSpectrum& ftTimeRamped = stftTimeRamped->getSpectrum( iSpec );
      result->addSpectrum( new SRSpectrum( ft, ftDerivative, ftTimeRamped ), new RawStftData::WindowLocation( stft->getWindowLocation( iSpec ) ) );
   }

   msg << Msg::Verbose << "Done" << Msg::EndReq;
   return RawStftData::Ptr( result );
}

} /// namespace WaveAnalysis
