#ifndef WAVELETTRANSFORM_H
#define WAVELETTRANSFORM_H

#include "RawPcmData.h"
#include "WindowFuncDef.h"
#include "WaveletContainer.h"


/// TODO: redesign WaveletTransform class:
/// Zeropadding, windowing (connected to fourier-size), offset (related to hop)
/// Prefiltering, extending, padding, applying windows, inverting
/// Create class that aggregates all this:
/// 1) work on RealVector, not on RawPcmData: to get real freq, might use wrapper class (y/n)?
/// 2) Make singleton that stores all initialised fftw algorithms (application lifetime wisdom)?
/// 3) How to deal with not invertible window functions?
/// SOL 3) Do not impl, or dont bother.
/// 4) Create Fourier filters (that work on time domain?), but can be applied on TR -> FR -> TR
/// SOL 4) Create FourierFilter working on FourierSpectrum,
/// 5)
// dontcompile

namespace WaveAnalysis
{

class WaveletTransform
{
   public:
      WaveletTransform( size_t size, size_t nLayers, size_t zeroPaadding, size_t hopRate, const WindowFuncDef& windowFunction );
      virtual ~WaveletTransform();

      WaveletContainer execute( const RawPcmData& data );

   private:
      void initialise();
      RealVector createWindowedData( const RawPcmData& data, size_t offset, const WindowFunction& winFunc, size_t fourierSize );

      size_t                 m_fourierSize;
      size_t                 m_nLayers;
      size_t                 m_zeroPadding;
      size_t                 m_hopRate;
      const WindowFuncDef*   m_windowFuncDef;
};

} /// namespace WaveAnalysis

#endif // WAVELETTRANSFORM_H
