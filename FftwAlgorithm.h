#ifndef FFTWALGORITHM_H
#define FFTWALGORITHM_H

/// Complex should always be included before the fftw header file!
#include <complex>
#include <fftw3.h>

#include "RawPcmData.h"
#include "Typedefs.h"

namespace WaveAnalysis {

/**
 * @class FftwAlgorithm
 * @brief Wrapper around the FFTW functionality
 */
class FftwAlgorithm
{
   public:
      /**
       * Obtain the number of degrees of freedom in the spectrum given the total Fourier size
       */
      static size_t getSpectrumDimension( size_t fourierSize );
      /**
       * Obtain the number of samples in the time domain given the degrees of freedom in the spectrum
       */
      static size_t getTimeDimension( size_t spectrumSize );

   public:
      /**
       * Construct a new FftwAlgorithm object
       * @param nSamples is the number of samples in the time domain.
       * (note that since it involves real data in the time domain, the complex fourier spectrum has n/2 degrees of freedom,
       * however, phase information is also present in the 0-frequency and the Nyquist frequency components).
       * @see getNumSamples and @see getSpectrumDimension
       */
      FftwAlgorithm( size_t nSamples );
      /**
       * Destructor
       */
      virtual ~FftwAlgorithm();

      /**
       * Get the number of samples in the time domain on which the Fourier transform operates
       */
      size_t getFourierSize() const;
      /**
       * Get the number of frequency components in the Fourier spectrum.
       */
      size_t getSpectrumDimension() const;

   /// TODO: temporary exposure for development of Spectral reassignment
   // protected:
   public:
      /**
       * Transform real valued data to complex Fourier spectrum.
       * @note The complex array is the working array of the algorithm and will be invalidated when the algorithm is
       * destructed or when a new transform is issued.
       */
      const Complex* transform( const double* timeData );
      /**
       * Transform complex valued spectral data to time data.
       * @note Similar note as @see transform. In addition, no scaling is performed.
       */
      const double* reverseTransform( const Complex* fourierData );

   /// TODO: consider hiding these methods
   public:
      /**
       * Get the pointer of the FFTW working array in the time domain
       */
      double* getTimeDataWorkingArray();
      /**
       * Get the pointer of the FFTW working array in the frequency domain
       */
      Complex* getFourierDataWorkingArray();
      /**
       * Forward (i.e. t -> f) transform on the private arrays
       */
      void transform();
      /**
       * Backward (i.e. f -> t) transform on the private arrays
       */
      void reverseTransform();

   private:
      size_t          m_nSamples;                  //! The number of samples in the time-domain
      double*         m_timeData;                  //! The working buffer of the -in- part of the transform
      fftw_complex*   m_fourierData;               //! The working buffer of the -out- part of the transform
      fftw_plan       m_planForward;               //! The FFTW plan for the time->spectrum transform
      fftw_plan       m_planBackward;              //! The FFTW plan for the spectrum->time transform
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline size_t FftwAlgorithm::getSpectrumDimension( size_t fourierSize )
{
   return fourierSize / 2 + 1;
}

inline size_t FftwAlgorithm::getTimeDimension( size_t spectrumSize )
{
   return ( spectrumSize - 1 ) * 2;
}

inline double* FftwAlgorithm::getTimeDataWorkingArray()
{
   return m_timeData;
}

inline Complex* FftwAlgorithm::getFourierDataWorkingArray()
{
   return reinterpret_cast< Complex* >( m_fourierData );
}

} /// namespace WaveAnalysis

/// todo: remove some code duplication

#endif // FFTWALGORITHM_H
