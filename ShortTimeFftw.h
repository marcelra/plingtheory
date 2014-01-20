#ifndef SHORTTIMEFFTW_H
#define SHORTTIMEFFTW_H

#include "FftwAlgorithm.h"
#include "WindowFuncDef.h"

/// Forward declares
class RawPcmData;
class TCanvas;

namespace WaveAnalysis
{

/**
 * @class ShortTimeFftw
 * @brief Transforms time-series data to spectral data.
 *
 * @see documentation of the constructor.
 */
class ShortTimeFftw : private FftwAlgorithm
{
   public:
      /**
       * Constructor
       * @param numSamples: the number of samples on which the FFT is based.
       * The Fourier transforms with size @param numSamples are calculated for time-series data @param data.
       * The number of Fourier spectra calculated is equal to the ceiled value of data.size() / numSamples.
       * Zero padding of the last data buffer is done whenever data.size() is not a multiple of numSamples.
       */
      ShortTimeFftw( size_t numSamples, const RawPcmData& data, const WindowFuncDef& windowFunction );

      /**
       * Destructor
       */
      ~ShortTimeFftw();

      /**
       * Get the number of spectra generated
       */
      size_t getNumSpectra() const;
      /**
       * Get spectrum indexed with @param index (non-const version)
       */
      FourierSpectrum& getSpectrum( size_t index );
      /**
       * Get spectrum indexed with @param index (const version)
       */
      const FourierSpectrum& getSpectrum( size_t index ) const;

      /**
       * Get the number of samples in the FFT buffer
       */
      using FftwAlgorithm::getFourierSize;
      using FftwAlgorithm::getSpectrumDimension;

   private:
      /**
       * Worker method (does the acutal work)
       */
      void calculateSpectra();
      RealVector applyWindow( const double* data, size_t offset );

      std::vector< FourierSpectrum > m_spectra;
      const RawPcmData&              m_data;
      const WindowFunction*          m_windowFunction;
};

} /// namespace WaveAnalysis

#endif // SHORTTIMEFFTW_H

