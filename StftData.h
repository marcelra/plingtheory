#ifndef STFTDATA_H
#define STFTDATA_H

#include "FourierSpectrum.h"

#include <vector>
#include <memory>

namespace WaveAnalysis
{

class StftData
{
   public:
      typedef std::auto_ptr< StftData > Ptr;
   public:
      /**
       * @class WindowLocation
       * @brief Helper class to calculate the window location of a STFT spectrum
       */
      class WindowLocation
      {
         public:
            WindowLocation( size_t firstSampleIndex, size_t lastSampleIndex );
            WindowLocation( const WindowLocation& other );
            WindowLocation& operator=( const WindowLocation& other );
            size_t getFirstSample() const;
            size_t getLastSample() const;
            size_t getCentre() const;
         private:
            size_t         m_first;
            size_t         m_last;
      };

   public:
      StftData( FourierConfig::CSPtr config );
      virtual ~StftData();

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

      /**
       * Access the settings of the Fourier algorithm used
       */
      const FourierConfig& getConfig() const;

   public:
      void addSpectrum( FourierSpectrum* spec, WindowLocation* windowLocation );

   private:
      std::vector< FourierSpectrum* >           m_transformedData;   //! The produced data
      std::vector< WindowLocation* >            m_windowLocations;   //! Location of the window in the original data
      FourierConfig::CSPtr                      m_config;            //! Settings of Fourier algorithm used

    /**
     * Blocked copy-constructor and assignment operator (default impl is not ok)
     */
    private:
      StftData( const StftData& other );
      StftData& operator=( const StftData& other );
};

} /// namespace WaveAnalysis

#endif // STFTDATA_H
