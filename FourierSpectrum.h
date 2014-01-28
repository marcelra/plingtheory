#ifndef FOURIERSPECTRUM_H
#define FOURIERSPECTRUM_H

#include "FourierConfig.h"
#include "RealVector.h"
#include "SamplingInfo.h"
#include "Typedefs.h"

namespace WaveAnalysis
{

/**
 * @class AdvancedFourierSpectrum
 * @brief Fourier spectrum created by the AdvancedFourierTransform class. Contains information needed for reversing
 * the transform.
 */
class FourierSpectrum : private ComplexVector
{
   public:
      FourierSpectrum( FourierConfig::CSPtr, const Complex* first, const Complex* last );

      using ComplexVector::at;
      using ComplexVector::operator[];
      using ComplexVector::front;
      using ComplexVector::back;
      using ComplexVector::size;
      using ComplexVector::begin;
      using ComplexVector::end;
      using ComplexVector::rbegin;
      using ComplexVector::rend;

      /**
       * Access the sampling info object
       */
      const SamplingInfo& getSamplingInfo() const;

      /**
       * Get the frequency of bin indexed by @param binIndex
       */
      double getFrequencyOfBin( size_t binIndex ) const;
      /**
       * Obtain frequencies for all bins in the spectrum
       */
      const RealVector& getFrequencies() const;
      /**
       * Get interpolated frequency
       */
      double getFrequency( double x ) const;

      /**
       * Get the number of samples (degrees of freedom) in the time domain (i.e. N in time -> N/2 + 1 in complex)
       */
      size_t getNumSamplesTimeDomain() const;

      /**
       * Get the magnitude of bin @param binIndex
       */
      double getMagnitudeInBin( size_t binIndex ) const;
      /**
       * Get the phase of bin @param binIndex
       */
      double getPhaseInBin( size_t binIndex ) const;

      /**
       * Produce a vector of magnitudes
       */
      RealVector getMagnitude() const;
      /**
       * Produce a vector of phases
       */
      RealVector getPhase() const;

      typedef std::auto_ptr< FourierSpectrum > Ptr;

   private:
      FourierConfig::CSPtr m_config;      //! Pointer to the configuration of the transform
};

} /// namespace WaveAnalysis

#endif // ADVANCEDFOURIERSPECTRUM_H
