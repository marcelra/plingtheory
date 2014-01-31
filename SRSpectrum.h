#ifndef SRSPECTRUM_H
#define SRSPECTRUM_H

#include "FourierSpectrum.h"

namespace WaveAnalysis
{

class SRSpectrum : public FourierSpectrum
{
   public:
      SRSpectrum( const FourierSpectrum& ft, const FourierSpectrum& ftDerivative, const FourierSpectrum& ftTimeRamp );

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

   private:
      RealVector        m_correctedFrequencies;
      RealVector        m_freqCorrections;
      RealVector        m_timeCorrections;
};

} /// namespace WaveAnalysis

#endif // SRSPECTRUM_H
