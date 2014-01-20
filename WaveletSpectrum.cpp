#include "WaveletSpectrum.h"

namespace WaveAnalysis
{

WaveletSpectrum::WaveletSpectrum( const SamplingInfo& samplingInfo,
                                  const WindowFunction& windowFunction,
                                  size_t zeroPadding,
                                  const std::complex< double >* first,
                                  const std::complex< double >* last ) :
   FourierSpectrum( samplingInfo, first, last ),
   m_winFunc( windowFunction ),
   m_zeroPadding( zeroPadding )
{}

WaveletSpectrum::~WaveletSpectrum()
{}



// WaveletSpectrum::WaveletSpectrum( const std::complex< double >* transformedDataPtr, size_t dataSize, const WindowFunction& windowFunction ) :
//    m_magnitudeArr( dataSize ),
//    m_phaseArr( dataSize ),
//    m_windowFunction( windowFunction )
// {
//    fillArrays( transformedDataPtr, dataSize );
// }
//
// WaveletSpectrum::WaveletSpectrum( const std::vector< std::complex< double > >& transformedData, const WindowFunction& windowFunction ) :
//    m_magnitudeArr( transformedData.size() ),
//    m_phaseArr( transformedData.size() ),
//    m_windowFunction( windowFunction )
// {
//    fillArrays( &transformedData[0], transformedData.size() );
// }
//
// void WaveletSpectrum::fillArrays( const std::complex< double >* transformedDataPtr, size_t dataSize )
// {
//    for ( size_t i = 0; i < dataSize; ++i )
//    {
//       m_magnitudeArr[i] = abs( transformedDataPtr[i] );
//       /// URGENT TODO: fill with angle of complex
//       m_phaseArr[i] = 0;
//    }
// }
//
// WaveletSpectrum::~WaveletSpectrum()
// {}
//
// const RealVector& WaveletSpectrum::getMagnitudeVec() const
// {
//    return m_magnitudeArr;
// }
//
// const RealVector& WaveletSpectrum::getPhaseVec() const
// {
//    return m_phaseArr;
// }

} /// namespace WaveAnalysis
