#ifndef SRSPECTRUM_H
#define SRSPECTRUM_H

#include "FourierSpectrum.h"
#include "RegularAccumArray.h"

namespace WaveAnalysis
{

/**
 * @class SrSpectrum
 * @brief Fourier spectrum with spectral reassignment corrections.
 */
class SrSpectrum : public FourierSpectrum
{
   public:
      /**
       * Constructor.
       * @param ft: the Fourier transform with the normal window function.
       * @param ftDerivate: the Fourier transform with the derivative window function.
       * @param ftTimeRamp: the Fourier transform with the time-ramped window function.
       */
      SrSpectrum( const FourierSpectrum& ft, const FourierSpectrum& ftDerivative, const FourierSpectrum& ftTimeRamp );

      /**
       * Get the frequency of bin indexed by @param binIndex.
       */
      double getFrequencyOfBin( size_t binIndex ) const;
      /**
       * Obtain frequencies for all bins in the spectrum.
       */
      const RealVector& getFrequencies() const;
      /**
       * Get interpolated frequency.
       */
      double getFrequency( double x ) const;

      /**
       * Get frequency corrections.
       */
      const RealVector& getFrequencyCorrections() const;
      /**
       * Get time corrections.
       */
      const RealVector& getTimeCorrections() const;

   public:
      /**
       * Rebin the corrected (frequency, magnitude) to the original Fourier bins.
       */
      Math::RegularAccumArray rebinToFourierLattice() const;

   private:
      RealVector        m_correctedFrequencies;          //! Corrected frequencies.
      RealVector        m_freqCorrections;               //! The frequency corrections (omega_hat).
      RealVector        m_timeCorrections;               //! The time corrections (t_hat).
};

} /// namespace WaveAnalysis

#endif // SRSPECTRUM_H
