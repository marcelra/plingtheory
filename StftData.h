#ifndef STFTDATA_H
#define STFTDATA_H

#include "FourierSpectrum.h"
#include "SRSpectrum.h"

#include <vector>
#include <memory>

namespace WaveAnalysis
{

/**
 * @class StftData
 * @brief Short-time Fourier transformed data container.
 * The Fourier transformed data are sorted with respect to time.
 * The WindowLocation subclass can be used to determine the time-range of the Fourier transform.
 */
class StftData
{
   public:
      /**
       * Readability typedef.
       */
      typedef std::auto_ptr< StftData > Ptr;
   public:
      /**
       * @class WindowLocation
       * @brief Helper class that remembers the window location of a STFT spectrum.
       */
      class WindowLocation
      {
         public:
            /**
             * Constructor. @param firstSampleIndex is the first sample on which the STFT window is applied.
             * @param lastSampleIndex is the last sample on which the STFT window is applied.
             */
            WindowLocation( size_t firstSampleIndex, size_t lastSampleIndex );
            /**
             * Get the first sample on which the window function was applied.
             */
            size_t getFirstSample() const;
            /**
             * Get the last sample on which the window function was applied.
             */
            size_t getLastSample() const;
            /**
             * Get the center of the window.
             */
            size_t getCentre() const;
         private:
            size_t         m_first;       //! First sample on which the window is applied.
            size_t         m_last;        //! Last sample on which the window is applied.
      };

   public:
      /**
       * Create an empty StftData object. The FourierConfig will be stored (@see FourierConfig).
       */
      StftData( FourierConfig::CSPtr config );
      /**
       * Destructor.
       */
      virtual ~StftData();

      /**
       * Get the total number of Fourier spectra produced.
       */
      size_t getNumSpectra() const;

      /**
       * Get the spectrum by index.
       */
      FourierSpectrum& getSpectrum( size_t spectrumIndex );
      const FourierSpectrum& getSpectrum( size_t spectrumIndex ) const;

      /**
       * Get the window location for spectrum @param spectrumIndex.
       */
      const WindowLocation& getWindowLocation( size_t spectrumIndex ) const;
      WindowLocation getWindowLocationNoOverlap( size_t spectrumIndex ) const;

      /**
       * Access the settings of the Fourier algorithm used.
       */
      const FourierConfig& getConfig() const;

   public:
      /**
       * Find out if the STFT data consists of reassigned spectra.
       */
      bool isReassigned() const;
      /**
       * Gets a SrSpectrum (asserts, when the spectra are not reassigned).
       */
      SRSpectrum& getSrSpectrum( size_t spectrumIndex );
      const SRSpectrum& getSrSpectrum( size_t spectrumIndex ) const;

   private:
      /**
       * Add another spectrum to the container. The added spectrum should be located after the last STFT data in time.
       */
      void addSpectrum( FourierSpectrum* spec, WindowLocation* windowLocation );

      /**
       * Friend class definitions.
       */
      friend class StftAlgorithm;
      friend class SpectralReassignmentTransform;

   private:
      std::vector< FourierSpectrum* >           m_transformedData;   //! The produced data
      std::vector< WindowLocation* >            m_windowLocations;   //! Location of the window in the original data
      FourierConfig::CSPtr                      m_config;            //! Settings of Fourier algorithm used

    /**
     * Blocked copy-constructor and assignment operator (default implementation is not ok).
     */
    private:
      StftData( const StftData& other );
      StftData& operator=( const StftData& other );
};

} /// namespace WaveAnalysis

#endif // STFTDATA_H
