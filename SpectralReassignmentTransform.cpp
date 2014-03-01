#include "SpectralReassignmentTransform.h"

#include "Logger.h"
#include "WindowFuncDef.h"

namespace WaveAnalysis
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SpectralReassignmentTransform::SpectralReassignmentTransform( const SamplingInfo& samplingInfo, size_t fourierSize, size_t numSamplesZeroPadding, double hopRate ) :
   m_stft( samplingInfo, fourierSize, HanningWindowFuncDef(), numSamplesZeroPadding, hopRate ),
   m_stftDerivative( samplingInfo, fourierSize, HanningDerivativeWindowFuncDef(), numSamplesZeroPadding, hopRate ),
   m_stftTimeRamped( samplingInfo, fourierSize, HanningTimeRampedWindowFuncDef(), numSamplesZeroPadding, hopRate )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// execute
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StftData::Ptr SpectralReassignmentTransform::execute( const RawPcmData& data )
{
   Logger msg( "SpectralReassignmentTransform" );
   msg << Msg::Verbose << "In execute..." << Msg::EndReq;

   /// Create result object container.
   StftData* result = new StftData( m_stft.getConfig() );

   msg << Msg::Verbose << "Executing the STFT transforms..." << Msg::EndReq;

   /// Calculate the STFTs for the different Windows (ordinary, derivative and time-ramped).
   StftData::Ptr stft = m_stft.execute( data );
   StftData::Ptr stftDerivative = m_stftDerivative.execute( data );
   StftData::Ptr stftTimeRamped = m_stftTimeRamped.execute( data );

   assert( stft->getNumSpectra() == stftDerivative->getNumSpectra() );
   assert( stft->getNumSpectra() == stftTimeRamped->getNumSpectra() );

   msg << Msg::Verbose << "Calculating the reassigned spectra." << Msg::EndReq;

   /// Create SrSpectrum objects from the three spectra. The SrSpectrum class will calculate the actual omega_hat and t_hat.
   for ( size_t iSpec = 0; iSpec < stft->getNumSpectra(); ++iSpec )
   {
      const FourierSpectrum& ft = stft->getSpectrum( iSpec );
      const FourierSpectrum& ftDerivative = stftDerivative->getSpectrum( iSpec );
      const FourierSpectrum& ftTimeRamped = stftTimeRamped->getSpectrum( iSpec );
      result->addSpectrum( new SRSpectrum( ft, ftDerivative, ftTimeRamped ), new StftData::WindowLocation( stft->getWindowLocation( iSpec ) ) );
   }

   msg << Msg::Verbose << "Done" << Msg::EndReq;

   /// Wrap pointer.
   return StftData::Ptr( result );
}

} /// namespace WaveAnalysis
