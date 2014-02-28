#ifndef STFTALGORITHM_H
#define STFTALGORITHM_H

#include "FourierTransform.h"
#include "FourierSpectrum.h"
#include "RawPcmData.h"
#include "StftData.h"

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
       * Constructor
       * Parameter function similar to the AdvancedFourierTransform
       * @param hopsPerWindow, number of hops per windowSize.
       */
      StftAlgorithm( const SamplingInfo& samplingInfo, size_t windowSize = 4096, const WindowFuncDef& windowFuncDef = HanningWindowFuncDef(), size_t numSamplesZeroPadding = 4096, double hopsPerWindow = 2 );

      /**
       * Execute an STFT transform on data
       */
      StftData::Ptr execute( const RawPcmData& data );
      RawPcmData::Ptr reverseExecute( const StftData& stftData );

      /**
       * Get the number of frequencies in the spectra
       */
      size_t getSpectrumDimension() const;

      /**
       * Get the configuration as constant shared pointer
       */
      FourierConfig::CSPtr getConfig() const;

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
      double                                   m_hopsPerWindow;     //! The hop-rate as given by the user
      FourierTransform                         m_transform;         //! The worker transform

   /**
    * Blocked copy-constructor and assigment operator
    */
   private:
      StftAlgorithm( const StftAlgorithm& other );
      StftAlgorithm& operator=( const StftAlgorithm& other );
};



} /// namespace WaveAnalysis

#endif // STFTALGORITHM_H
