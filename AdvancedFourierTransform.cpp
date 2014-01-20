#include "AdvancedFourierTransform.h"

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
AdvancedFourierTransform::AdvancedFourierTransform( const SamplingInfo& samplingInfo, size_t windowSize, const WindowFuncDef& windowFuncDef, size_t numSamplesZeroPadding ) :
   m_samplingInfo( samplingInfo ),
   m_windowSize( windowSize ),
   m_windowFuncDef( windowFuncDef.clone() ),
   m_windowFunction( windowFuncDef.createWindowFunction( windowSize ) ),
   m_numSamplesZeroPadding( numSamplesZeroPadding ),
   m_algorithm( getTotalFourierSize() ),
   m_needsInitTimeArr( true )
{
   initFrequencyList();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// initFftwArrays
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AdvancedFourierTransform::initFftwArrays()
{
   double* arr = m_algorithm.getTimeDataWorkingArray();
   for ( size_t i = 0; i < getTotalFourierSize(); ++i )
   {
      arr[i] = 0;
   }
   m_needsInitTimeArr = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// initFrequencyList
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AdvancedFourierTransform::initFrequencyList()
{
   size_t numFrequencies = m_algorithm.getSpectrumDimension();
   m_frequencies.resize( numFrequencies );
   double lowestFrequency = getLowestFrequency();
   for ( size_t i = 0; i < numFrequencies; ++i )
   {
      m_frequencies[i] = lowestFrequency * i;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// transform
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AdvancedFourierSpectrum::Ptr AdvancedFourierTransform::transform( const double* data )
{
   if ( m_needsInitTimeArr )
   {
      initFftwArrays();
   }

   double* arr = m_algorithm.getTimeDataWorkingArray();
   const WindowFunction& winFunc = getWindowFunction();

   for ( size_t iSample = 0; iSample < getWindowSize(); ++iSample )
   {
      arr[iSample] = data[iSample] * winFunc.calc( iSample );
   }

   m_algorithm.transform();

   Complex* resultFirst = m_algorithm.getFourierDataWorkingArray();
   Complex* resultLast = m_algorithm.getFourierDataWorkingArray() + m_algorithm.getSpectrumDimension();
   return AdvancedFourierSpectrum::Ptr( new AdvancedFourierSpectrum( *this, resultFirst, resultLast ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// transform (reverse)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVectorPtr AdvancedFourierTransform::transform( const AdvancedFourierSpectrum& spectrum )
{
   /// Check conditions
   assert( isInvertible() );
   assert( spectrum.size() == m_algorithm.getSpectrumDimension() );

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
   size_t windowSize = getWindowSize();
   size_t totalFourierSize = getWindowSize() + getNumSamplesZeroPadding();

   /// Build result, this is still multiplied by WindowFunction
   double* first = m_algorithm.getTimeDataWorkingArray();
   double* last = m_algorithm.getTimeDataWorkingArray() + windowSize;
   RealVector* result = new RealVector( first, last );

   /// Undo windowing
   const WindowFunction& winFunc = getWindowFunction();
   for ( size_t iSample = 0; iSample < windowSize; ++iSample )
   {
      (*result)[iSample] /= ( winFunc.calc( iSample ) * totalFourierSize );
   }

   return RealVectorPtr( result );
}

} /// namespace WaveAnalysis
