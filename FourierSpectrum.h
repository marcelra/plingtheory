#ifndef FOURIERSPECTRUM_H
#define FOURIERSPECTRUM_H

#include "FourierConfig.h"
#include "RealVector.h"
#include "SamplingInfo.h"
#include "Typedefs.h"

#include <memory>

/// Forward declarations.
namespace WaveAnalysis
{
class WindowLocation;
}

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
      /**
       * Constructor.
       * @param fourierConfig: the configuration of the Fourier transform that created the spectrum.
       * @param first: starting iterator to the complex data.
       * @param last: end iteration to the complex data.
       * @param windowLocation: Specify the window location (@see class WindowLocation). Pointer will be owned by this class.
       */
      FourierSpectrum( FourierConfig::CSPtr fourierConfig, const Complex* first, const Complex* last, WindowLocation* windowLocation = 0 );
      /**
       * Copy-constructor and assignment operator.
       */
      FourierSpectrum( const FourierSpectrum& other );
      FourierSpectrum& operator=( const FourierSpectrum& other );
      /**
       * Destructor.
       */
      virtual ~FourierSpectrum();
      /**
       * Clone method.
       */
      virtual FourierSpectrum* clone() const;

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
       * Access the fourier configuration
       */
      const FourierConfig& getConfig() const;

      /**
       * Get the frequency of bin indexed by @param binIndex
       */
      virtual double getFrequencyOfBin( size_t binIndex ) const;
      /**
       * Obtain frequencies for all bins in the spectrum
       */
      virtual const RealVector& getFrequencies() const;
      /**
       * Get interpolated frequency
       */
      virtual double getFrequency( double x ) const;

      /**
       * Get the number of samples (degrees of freedom) in the time domain (i.e. N in time -> N/2 + 1 in complex)
       */
      size_t getNumSamplesTimeDomain() const;

      /**
       * Get the location of the Fourier window. Might yield a zero pointer, depending on the algorithm that created this spectrum.
       */
      const WindowLocation* getWindowLocation() const;
      /**
       * Set the window location of the Fourier window. WindowLocation instance will be owned by this instance.
       */
      void setWindowLocation( WindowLocation* windowLocation );

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

      typedef std::unique_ptr< FourierSpectrum > Ptr;

   private:
      FourierConfig::CSPtr                    m_config;         //! Pointer to the configuration of the transform.
      std::unique_ptr< const WindowLocation > m_windowLocation; //! Pointer to the window location instance.
};

} /// namespace WaveAnalysis

#endif // ADVANCEDFOURIERSPECTRUM_H
