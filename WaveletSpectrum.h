#ifndef WAVELETSPECTRUM_H
#define WAVELETSPECTRUM_H

#include "FourierSpectrum.h"
#include "RealVector.h"
#include "WindowFunction.h"

#include <complex>

namespace WaveAnalysis
{

class WaveletSpectrum : public FourierSpectrum
{
   public:
      WaveletSpectrum( const SamplingInfo& samplingInfo,
                       const WindowFunction& windowFunction,
                       size_t zeroPadding,
                       const std::complex< double >* first,
                       const std::complex< double >* last );
      virtual ~WaveletSpectrum();

   private:
      const WindowFunction&    m_winFunc;
      size_t                   m_zeroPadding;
};

// class WaveletSpectrum : public FourierSpectrum
// {
//    public:
//       WaveletSpectrum( const std::complex< double >* transformDataPtr, size_t dataSize, const WindowFunction& windowFunction );
//       WaveletSpectrum( const std::vector< std::complex< double > >& transformedData, const WindowFunction& windowFunction );
//
//       virtual ~WaveletSpectrum();
//
//    public:
//       size_t getSize() const;
//       double getMagnitude( size_t index ) const;
//       double getPhase( size_t index ) const;
//
//       const RealVector& getMagnitudeVec() const;
//       const RealVector& getPhaseVec() const;
//
//    private:
//       void fillArrays( const std::complex< double >* transformedDataPtr, size_t dataSize );
//
//    private:
//       RealVector              m_magnitudeArr;
//       RealVector              m_phaseArr;
//       const WindowFunction&   m_windowFunction;
// };
//
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// /// Inline methods
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// inline size_t WaveletSpectrum::getSize() const
// {
//    return m_magnitudeArr.size();
// }
//
// inline double WaveletSpectrum::getMagnitude( size_t index ) const
// {
//    return m_magnitudeArr[ index ];
// }
//
// inline double WaveletSpectrum::getPhase( size_t index ) const
// {
//    return m_phaseArr[ index ];
// }
//
} /// namespace WaveAnalysis

#endif // WAVELETSPECTRUM_H
