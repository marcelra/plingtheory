#ifndef STFTALGORITHM_H
#define STFTALGORITHM_H

#include "FourierTransform.h"
#include "FourierSpectrum.h"
#include "RawPcmData.h"

namespace WaveAnalysis
{

/**
 * @class StftAlgorithm
 * @brief Short-time Fourier algorithm. Transforms a RawPcmData object with hoprate and using the AdvancedFourierTransform
 *
 * Combined algortihm-data class. Destroying the algorithm will invalidate the data & single execution.
 */
class StftAlgorithm
{
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
      /**
       * Constructor
       * Parameter function similar to the AdvancedFourierTransform
       * @param hopsPerWindow, number of hops per windowSize.
       */
      StftAlgorithm( const SamplingInfo& samplingInfo, size_t windowSize = 4096, const WindowFuncDef& windowFuncDef = HanningWindowFuncDef(), size_t numSamplesZeroPadding = 4096, double hopsPerWindow = 2 );
      /**
       * Destructor
       */
      virtual ~StftAlgorithm();

      /**
       * Execute an STFT transform on data
       */
      void execute( const RawPcmData& data );
      RawPcmData::Ptr reverseExecute();

      /**
       * Check whether the algorithm has executed
       */
      bool hasExecuted() const;

      /**
       * Get the total number of Fourier spectra produced
       */
      size_t getNumSpectra() const;

      /**
       * Get the number of frequencies in the spectra
       */
      size_t getSpectrumDimension() const;

      /**
       * Get the spectrum by index
       */
      FourierSpectrum& getSpectrum( size_t spectrumIndex );
      const FourierSpectrum& getSpectrum( size_t spectrumIndex ) const;

      const WindowLocation& getWindowLocation( size_t spectrumIndex ) const;
      WindowLocation getWindowLocationNoOverlap( size_t spectrumIndex ) const;

      /**
       * Find the spectrum closed to sample with index @param sapleIndex
       */
      FourierSpectrum& getClosestSpectrumAt( size_t sampleIndex );
      const FourierSpectrum& getClosestSpectrumAt( size_t sampleIndex ) const;

   private:
      /**
       * Extends the @param RawPcmData with zeroes to fit the window size (needed for the last batches)
       */
      RealVector extendDataWithZeros( const RawPcmData& data, size_t currentIndex );
      /**
       * Get the shift in sampls corresponding to the hop rate
       */
      double getHopShift() const;

   private:
      bool                                     m_hasExecuted;       //! Bool indicating whether the algorithm has executed.
      double                                   m_hopsPerWindow;     //! The hop-rate as given by the user
      FourierTransform                         m_transform;         //! The worker transform
      std::vector< FourierSpectrum* >          m_transformedData;   //! The produced data
      std::vector< WindowLocation >            m_windowLocations;

   /**
    * Blocked copy-constructor and assigment operator
    */
   private:
      StftAlgorithm( const StftAlgorithm& other );
      StftAlgorithm& operator=( const StftAlgorithm& other );
};



} /// namespace WaveAnalysis

#endif // STFTALGORITHM_H
