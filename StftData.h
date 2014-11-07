#ifndef STFTDATA_H
#define STFTDATA_H

#include "FourierSpectrum.h"

#include <vector>
#include <memory>

/// Forward declarations
namespace WaveAnalysis
{
class SrSpectrum;
class WindowLocation;
}

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
      typedef std::unique_ptr< StftData > Ptr;

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
       * Copy constructor.
       */
      StftData( const StftData& other );

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
      SrSpectrum& getSrSpectrum( size_t spectrumIndex );
      const SrSpectrum& getSrSpectrum( size_t spectrumIndex ) const;

   private:
      /**
       * Add another spectrum to the container. The added spectrum should be located after the last STFT data in time.
       */
      void addSpectrum( FourierSpectrum* spec );

      /**
       * Friend class definitions.
       */
      friend class StftAlgorithm;
      friend class SpectralReassignmentTransform;

   private:
      std::vector< FourierSpectrum* >           m_transformedData;   //! The produced data
      FourierConfig::CSPtr                      m_config;            //! Settings of Fourier algorithm used

    /**
     * Blocked assignment operator (default implementation is not ok).
     */
    private:
      StftData& operator=( const StftData& other );
};

} /// namespace WaveAnalysis

#endif // STFTDATA_H
