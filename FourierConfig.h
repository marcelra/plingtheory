#ifndef FOURIERCONFIG_H
#define FOURIERCONFIG_H

#include "WindowFuncDef.h"

#include "boost/shared_ptr.hpp"

class SamplingInfo;

namespace WaveAnalysis
{

class FourierConfig
{
   public:
      typedef boost::shared_ptr< const FourierConfig > CSPtr;
   public:
      /**
       * Constructor. Create a FourierTransform based on a window size, @param windowSize, with a window function, @windowFuncDef and
       * number of zero padding samples, @param numSamplesZeroPadding. The sampling information, @param samplingInfo, is needed for the
       * calculation of frequencies.
       */
      FourierConfig( const SamplingInfo& samplingInfo, size_t windowSize = 4096, const WindowFuncDef& windowFuncDef = HanningWindowFuncDef(), size_t numSamplesZeroPadding = 0 );

      /**
       * Obtain a reference to the SamplingInfo object.
       */
      const SamplingInfo&	 getSamplingInfo() const;
      /**
       * Get the size of the windowing function.
       */
      size_t getWindowSize() const;
      /**
       * Get the number of zero padding samples appended to the windowed data.
       */
      size_t getNumSamplesZeroPadding() const;
      /**
       * Get the total number of samples used in the transform.
       */
      size_t getTotalFourierSize() const;
      /**
       * Get a reference to the window function.
       */
      const WindowFunction& getWindowFunction() const;

      /**
       * Check whether the Fourier transform is invertible. This depends on the window function used. Window functions
       * are not invertible when somewhere w(t) = 0
       */
      bool isInvertible() const;
      /**
       * Gets the lowest non-zero frequency.
       */
      double getLowestFrequency() const;
      /**
       * Get the width of a frequency bin.
       */
      double getFrequencyBinWidth() const;
      /**
       * Returns a vector containing all the frequencies that are probed by the transform.
       */
      const RealVector& getSpectrumFrequencies() const;
      /**
       * Returns the number of frequencies in spectrum
       */
      size_t getSpectrumDimension() const;

   private:
      /**
       * Initialise the frequency list
       */
      void initFrequencyList();

   private:
      const SamplingInfo&        m_samplingInfo;             //! SamplingInfo object
      size_t                     m_windowSize;               //! Windowsize
      const WindowFuncDef*       m_windowFuncDef;            //! WindowFunction definition object
      const WindowFunction*      m_windowFunction;           //! Instantiated window function
      size_t                     m_numSamplesZeroPadding;    //! Number of zero padding samples
      RealVector                 m_frequencies;              //! Frequency list

   /**
    * Blocked copy-constructor and assignment operators (default impl does not satisfy)
    */
   private:
      FourierConfig( const FourierConfig& other );
      FourierConfig& operator=( const FourierConfig& other );

   friend bool operator!=( const FourierConfig& fc1, const FourierConfig& fc2 );
   friend bool operator==( const FourierConfig& fc1, const FourierConfig& fc2 );
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Compare two configs
 */
bool operator!=( const FourierConfig& fc1, const FourierConfig& fc2 );
bool operator==( const FourierConfig& fc1, const FourierConfig& fc2 );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline const SamplingInfo& FourierConfig::getSamplingInfo() const
{
   return m_samplingInfo;
}

inline size_t FourierConfig::getWindowSize() const
{
   return m_windowSize;
}

inline size_t FourierConfig::getNumSamplesZeroPadding() const
{
   return m_numSamplesZeroPadding;
}

inline size_t FourierConfig::getTotalFourierSize() const
{
   return m_windowSize + m_numSamplesZeroPadding;
}

inline const WindowFunction& FourierConfig::getWindowFunction() const
{
   return *m_windowFunction;
}

inline bool FourierConfig::isInvertible() const
{
   return m_windowFuncDef->isInvertible();
}

inline const RealVector& FourierConfig::getSpectrumFrequencies() const
{
   return m_frequencies;
}

} /// WaveAnalysis

#endif // FOURIERCONFIG_H
