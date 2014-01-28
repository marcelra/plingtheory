#ifndef RAWSTFTDATA_H
#define RAWSTFTDATA_H

#include "FourierSpectrum.h"

#include <vector>
#include <memory>

namespace WaveAnalysis
{

class RawStftData
{
   public:
      typedef std::auto_ptr< RawStftData > Ptr;
   public:
      /**
       * @class WindowLocation
       * @brief Helper class to calculate the window location of a STFT spectrum
       */
      class WindowLocation
      {
         public:
            WindowLocation( size_t firstSampleIndex, size_t lastSampleIndex );
            size_t getFirstSample() const;
            size_t getLastSample() const;
            size_t getCentre() const;
         private:
            size_t         m_first;
            size_t         m_last;
      };

   public:
      RawStftData();
      virtual ~RawStftData();

      /**
       * Get the total number of Fourier spectra produced
       */
      size_t getNumSpectra() const;

      /**
       * Get the spectrum by index
       */
      FourierSpectrum& getSpectrum( size_t spectrumIndex );
      const FourierSpectrum& getSpectrum( size_t spectrumIndex ) const;

      /**
       * Get the window location for spectrum @param spectrumIndex
       */
      const WindowLocation& getWindowLocation( size_t spectrumIndex ) const;
      WindowLocation getWindowLocationNoOverlap( size_t spectrumIndex ) const;

   public:
      void addSpectrum( FourierSpectrum* spec, WindowLocation* windowLocation );

   private:
      std::vector< FourierSpectrum* >          m_transformedData;   //! The produced data
      std::vector< WindowLocation* >            m_windowLocations;
};

} /// namespace WaveAnalysis

#endif // RAWSTFTDATA_H
