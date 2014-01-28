#include "FourierTransform.h"

namespace WaveAnalysis
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Notes:
/// * Builds frequency list during initialisation
/// * Works on the internal FFTW algorithms for efficiency (less copy-operations of data)
/// Initialises the whole internal array with zeroes so that zero-padding is for free
/// During reverse transform some of these zero-padded samples may become small, but non-zero, for this reason the array
/// needs reinitialisation after a reverse transform.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FourierTransform::FourierTransform( const SamplingInfo& samplingInfo, size_t windowSize, const WindowFuncDef& windowFuncDef, size_t numSamplesZeroPadding ) :
   m_config( new FourierConfig( samplingInfo, windowSize, windowFuncDef, numSamplesZeroPadding ) ),
   m_algorithm( m_config->getTotalFourierSize() ),
   m_needsInitTimeArr( true )
{}

FourierTransform::FourierTransform( FourierConfig::CSPtr config ) :
   m_config( config ),
   m_algorithm( m_config->getTotalFourierSize() ),
   m_needsInitTimeArr( true )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// initFftwArrays
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FourierTransform::initFftwArrays()
{
   double* arr = m_algorithm.getTimeDataWorkingArray();
   for ( size_t i = 0; i < m_config->getTotalFourierSize(); ++i )
   {
      arr[i] = 0;
   }
   m_needsInitTimeArr = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// transform
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FourierSpectrum::Ptr FourierTransform::transform( const double* data )
{
   if ( m_needsInitTimeArr )
   {
      initFftwArrays();
   }

   double* arr = m_algorithm.getTimeDataWorkingArray();
   const WindowFunction& winFunc = m_config->getWindowFunction();

   for ( size_t iSample = 0; iSample < m_config->getWindowSize(); ++iSample )
   {
      arr[iSample] = data[iSample] * winFunc.calc( iSample );
   }

   m_algorithm.transform();

   Complex* resultFirst = m_algorithm.getFourierDataWorkingArray();
   Complex* resultLast = m_algorithm.getFourierDataWorkingArray() + m_algorithm.getSpectrumDimension();
   return FourierSpectrum::Ptr( new FourierSpectrum( m_config, resultFirst, resultLast ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// transform (reverse)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVectorPtr FourierTransform::transform( const FourierSpectrum& spectrum )
{
   /// Check conditions
   assert( m_config->isInvertible() );
   assert( spectrum.size() == m_config->getSpectrumDimension() );

   /// Fill complex Fftw array with spectrum data
   Complex* arr = m_algorithm.getFourierDataWorkingArray();
   for ( size_t i = 0; i < spectrum.size(); ++i )
   {
      arr[i] = spectrum[i];
   }

   /// Transform
   m_algorithm.reverseTransform();
   m_needsInitTimeArr = true;

   /// Get sizes
   size_t windowSize = m_config->getWindowSize();
   size_t totalFourierSize = m_config->getTotalFourierSize();

   /// Build result, this is still multiplied by WindowFunction
   double* first = m_algorithm.getTimeDataWorkingArray();
   double* last = m_algorithm.getTimeDataWorkingArray() + windowSize;
   RealVector* result = new RealVector( first, last );

   /// Undo windowing
   const WindowFunction& winFunc = m_config->getWindowFunction();
   for ( size_t iSample = 0; iSample < windowSize; ++iSample )
   {
      (*result)[iSample] /= ( winFunc.calc( iSample ) * totalFourierSize );
   }

   return RealVectorPtr( result );
}

} /// namespace WaveAnalysis
