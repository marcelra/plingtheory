#ifndef ADVANCEDFOURIERTRANSFORM_H
#define ADVANCEDFOURIERTRANSFORM_H

#include "AdvancedFourierSpectrum.h"
#include "FftwAlgorithm.h"
#include "SamplingInfo.h"
#include "WindowFuncDef.h"

namespace WaveAnalysis
{

/**
 * @class AdvancedFourierTransform
 * @brief Class for applying fourier transforms to data. Supports windowing and zero-padding.
 *
 * The user is in control of matching the data with the correct sampling information.
 * Altough the reverse transformation is able to transform any Fourier spectrum, results will be wrong in general if the
 * window function does not match the window function used to create the spectrum.
 */
class AdvancedFourierTransform
{
   public:
      /**
       * Constructor
       * @param SamplingInfo: provides the SamplingInfo object, needed for calculating frequencies.
       * @param windowSize: the size of the windowing function.
       * @param windowFunction: window function definition (the window function should be invertible in order to be able
       *        to perform reverse transformations.
       * @param numSamplesZeroPadding: the number of samples (zeroes) that are appended to the windowed data.
       */
      AdvancedFourierTransform( const SamplingInfo& samplingInfo, size_t windowSize = 4096, const WindowFuncDef& windowFuncDef = HanningWindowFuncDef(), size_t numSamplesZeroPadding = 0 );

      /**
       * Transform double array @param data. The array shoud at least be greater than the number of window samples
       */
      AdvancedFourierSpectrum::Ptr transform( const double* data );
      /**
       * Reverse transform of Fourier spectrum @param spectrum. Asserts if the window function is not invertible.
       */
      RealVectorPtr transform( const AdvancedFourierSpectrum& spectrum );

      /**
       * Obtain a reference to the SamplingInfo object.
       */
      const SamplingInfo&		   getSamplingInfo() const;
      /**
       * Get the size of the windowing function.
       */
      size_t 				      getWindowSize() const;
      /**
       * Get the number of zero padding samples appended to the windowed data.
       */
      size_t 				  		getNumSamplesZeroPadding() const;
      /**
       * Get the total number of samples used in the transform.
       */
      size_t                    getTotalFourierSize() const;
      /**
       * Get a reference to the window function.
       */
      const WindowFunction& 	   getWindowFunction() const;

      /**
       * Check whether the Fourier transform is invertible.
       */
      bool                      isInvertible() const;
      /**
       * Gets the lowest non-zero frequency.
       */
      double                    getLowestFrequency() const;
      /**
       * Returns a vector containing all the frequencies that are probed by the transform.
       */
      const RealVector&         getSpectrumFrequencies() const;
      /**
       * Returns the number of frequencies
       */
      size_t getSpectrumDimension() const;

   private:
      /**
       * Initialise the internal FftwAlgorithm time-domain array with zeroes.
       */
      void initFftwArrays();
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
      FftwAlgorithm              m_algorithm;                //! FFTW worker algorithm
      bool                       m_needsInitTimeArr;         //! Bool indicating that the internal FFTW algorithm arrays should be reinitialised to zeroes
      RealVector                 m_frequencies;              //! Frequency list
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline const SamplingInfo& AdvancedFourierTransform::getSamplingInfo() const
{
   return m_samplingInfo;
}

inline size_t AdvancedFourierTransform::getWindowSize() const
{
   return m_windowSize;
}

inline size_t AdvancedFourierTransform::getNumSamplesZeroPadding() const
{
   return m_numSamplesZeroPadding;
}

inline size_t AdvancedFourierTransform::getTotalFourierSize() const
{
   return m_windowSize + m_numSamplesZeroPadding;
}

inline const WindowFunction& AdvancedFourierTransform::getWindowFunction() const
{
   return *m_windowFunction;
}

inline bool AdvancedFourierTransform::isInvertible() const
{
   return m_windowFuncDef->isInvertible();
}

inline double AdvancedFourierTransform::getLowestFrequency() const
{
   return m_samplingInfo.getSamplingRate() / getTotalFourierSize();
}

inline const RealVector& AdvancedFourierTransform::getSpectrumFrequencies() const
{
   return m_frequencies;
}

inline size_t AdvancedFourierTransform::getSpectrumDimension() const
{
   return m_frequencies.size();
}

} /// namespace WaveAnalysis

#endif // ADVANCEDFOURIERTRANSFORM_H
