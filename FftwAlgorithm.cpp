#include "FftwAlgorithm.h"
#include "Logger.h"

#include <algorithm>

/// Anonymous namespace
namespace
{
   /// Helper struct for STL for_each algorithm
   /// Used for 1/sqrt(n) scaling
   template <class T>
   struct Scale
   {
      Scale( double factor ) : factor( factor ) {}
      void operator() ( T& x )
      {
         x *= factor;
      }
      double factor;
   };
}

namespace WaveAnalysis {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FftwAlgorithm::FftwAlgorithm( size_t nSamples ) :
   m_nSamples( nSamples )
{
   Logger msg( "FftwAlgorithm" );
   msg << Msg::Debug << "In FftwAlgorithm constructor..." << Msg::EndReq;

   /// Allocate working buffers
   m_timeData = (double*) fftw_malloc( sizeof(double) * m_nSamples );
   m_fourierData = (fftw_complex*) fftw_malloc( sizeof(fftw_complex) * ( getSpectrumDimension() ) );

   /// Construct plans
   msg << Msg::Info << "Constructing forward and backward plans for " << m_nSamples << " samples... " << Msg::EndReq;
   m_planForward  = fftw_plan_dft_r2c_1d( m_nSamples, m_timeData, m_fourierData, FFTW_ESTIMATE );
   m_planBackward = fftw_plan_dft_c2r_1d( m_nSamples, m_fourierData, m_timeData, FFTW_ESTIMATE );
   msg << Msg::Info << "Done." << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FftwAlgorithm::~FftwAlgorithm()
{
   fftw_destroy_plan( m_planForward );
   fftw_destroy_plan( m_planBackward );
   fftw_free( m_timeData );
   fftw_free( m_fourierData );

   Logger msg( "FftwAlgorithm" );
   msg << Msg::Debug << "Leaving FftwAlgorithm destructor." << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getNumSamples
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t FftwAlgorithm::getFourierSize() const
{
   return m_nSamples;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getSpectrumDimension
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t FftwAlgorithm::getSpectrumDimension() const
{
   return FftwAlgorithm::getSpectrumDimension( m_nSamples );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// transform (time->spectrum)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FourierSpectrum::Ptr FftwAlgorithm::transform( const RawPcmData& timeData, size_t offset )
{
   /// This is considered an error
   if ( offset > timeData.size() )
   {
      assert( false );
   }
   /// Check if padding is needed
   size_t lastSamplePos = offset + getFourierSize();
   double* paddedDataSet = 0;
   if ( lastSamplePos > timeData.size() )
   {
      /// Construct data set with zero-padding
      double* paddedDataSet = new double[ getFourierSize() ];
      size_t iPad = 0;
      for ( size_t iSample = offset; iSample < timeData.size(); ++iSample, ++iPad )
      {
         paddedDataSet[iPad] = timeData[iSample];
      }
      size_t numSamples = getFourierSize();
      for ( ; iPad < numSamples; ++iPad )
      {
         paddedDataSet[iPad] = 0;
      }
   }
   /// Transform
   const Complex* first = transform( paddedDataSet? paddedDataSet : &timeData[offset] );
   /// Copy data
   FourierSpectrum::Ptr result( new FourierSpectrum( timeData.getSamplingInfo(), first, first + getSpectrumDimension() ) );
   /// Scale by 1/sqrt(n)
   ::Scale< Complex > scale( 1.0 / sqrt( getFourierSize() ) );
   std::for_each( result->begin(), result->end(), scale );
   /// Clear padded data set
   if ( paddedDataSet )
   {
      delete paddedDataSet;
   }
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// transform (spectrum->time)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RawPcmData::Ptr FftwAlgorithm::transform( const FourierSpectrum& fourierData )
{
   /// Transform
   const double* first = reverseTransform( &fourierData[0] );
   /// Copy data
   RawPcmData::Ptr result ( new RawPcmData( fourierData.getSamplingInfo(), first, first + getFourierSize() ) );
   /// Scale by 1/sqrt(n)
   ::Scale< double > scale(  1.0 / sqrt( getFourierSize() ) );
   std::for_each( result->begin(), result->end(), scale );
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// transform
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Complex* FftwAlgorithm::transform( const double* timeData )
{
   /// Copy time data to working buffer
   std::copy( timeData, timeData + m_nSamples, m_timeData );
   /// Execute the transform
   fftw_execute( m_planForward );
   return reinterpret_cast< Complex* >( m_fourierData );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// reverseTransform
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const double* FftwAlgorithm::reverseTransform( const Complex* fourierData )
{
   /// Copy fourier data to working buffer
   const double* double_fourierData = reinterpret_cast< const double* >( fourierData );
   /// Factor of two due to counting degrees of freedom (i.e. real + imag)
   /// Note that 2*getSpectrumDimension == getNumSamples + 2
   size_t nParameters = 2 * getSpectrumDimension();
   std::copy( double_fourierData, double_fourierData + nParameters, reinterpret_cast< double* >(m_fourierData) );
   /// Execute the transform
   fftw_execute( m_planBackward );
   return m_timeData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// transform
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FftwAlgorithm::transform()
{
   fftw_execute( m_planForward );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// reverseTransform
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FftwAlgorithm::reverseTransform()
{
   fftw_execute( m_planBackward );
}

} /// namespace WaveAnalysis
