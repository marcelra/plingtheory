#ifndef FOURIERSPECTRUM_H
#define FOURIERSPECTRUM_H

#include "SamplingInfo.h"
#include "RealVector.h"

#include <vector>
#include <complex>
#include <cassert>
#include <memory>

namespace WaveAnalysis {

/**
 * @class FourierSpectrum
 * @brief Class that holds a fourier spectrum.
 */
class FourierSpectrum : private std::vector< std::complex<double> >
{
   public:
      /**
       * Initialse fourier spectrum with array std::complex<double> given by @param first and @param last
       * @see similar std::vector constructor
       */
      FourierSpectrum( const SamplingInfo& samplingInfo, const std::complex<double>* first, const std::complex<double>* last );

      /**
       * Destructor
       */
      virtual ~FourierSpectrum();

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
      std::vector< double > getFrequencies() const;
      /**
       * Get interpolated frequency
       */
      double getFrequency( double x ) const;

      /**
       * Get the number of samples (degrees of freedom) in the time domain (i.e. N in time -> N/2 + 1 in complex)
       */
      size_t getNumSamplesTimeDomain() const;

      /**
       * Get the magnitude of the spectrum
       */
      RealVector getMagnitude() const;
      /**
       * Get the phase of the spectrum
       */
      RealVector getPhase() const;

      /**
       * Typedef of auto_ptr (readability)
       */
      typedef std::auto_ptr< FourierSpectrum > Ptr;

      /**
       * Export of vector interface
       */
      typedef std::vector< std::complex<double> > vecC;
      using vecC::at;
      using vecC::operator[];
      using vecC::front;
      using vecC::back;
      using vecC::size;
      using vecC::begin;
      using vecC::end;
      using vecC::rbegin;
      using vecC::rend;

   private:
      SamplingInfo         m_samplingInfo;         //! Sampling information
      double               m_lowestFreq;           //! Lowest frequency in the spectrum
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline query methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline const SamplingInfo& FourierSpectrum::getSamplingInfo() const
{
   return m_samplingInfo;
}

inline size_t FourierSpectrum::getNumSamplesTimeDomain() const
{
   assert( size() > 0 );
   return (size() - 1)*2;
}


} /// namespace WaveAnalysis

#endif // FOURIERSPECTRUM_H
