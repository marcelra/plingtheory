#ifndef FOURIERTRANSFORM_H
#define FOURIERTRANSFORM_H

#include "FourierConfig.h"
#include "FourierSpectrum.h"
#include "FftwAlgorithm.h"
#include "SamplingInfo.h"
#include "WindowFuncDef.h"

#include "boost/shared_ptr.hpp"

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
class FourierTransform
{
   public:
      typedef boost::shared_ptr< FourierTransform > SPtr;

   public:
      /**
       * Constructor
       * @param SamplingInfo: provides the SamplingInfo object, needed for calculating frequencies.
       * @param windowSize: the size of the windowing function.
       * @param windowFunction: window function definition (the window function should be invertible in order to be able
       *        to perform reverse transformations.
       * @param numSamplesZeroPadding: the number of samples (zeroes) that are appended to the windowed data.
       */
      FourierTransform( const SamplingInfo& samplingInfo, size_t windowSize = 4096, const WindowFuncDef& windowFuncDef = HanningWindowFuncDef(), size_t numSamplesZeroPadding = 0 );

      /**
       * Constructor
       * @param config: a constant shared pointer of a initialised FourierConfig
       */
      FourierTransform( FourierConfig::CSPtr config );

      /**
       * Transform double array @param data. The array shoud at least be greater than the number of window samples
       */
      FourierSpectrum::Ptr transform( const double* data );
      /**
       * Reverse transform of Fourier spectrum @param spectrum. Asserts if the window function is not invertible.
       */
      RealVectorPtr transform( const FourierSpectrum& spectrum );

      const FourierConfig& getConfig() const;

   private:
      /**
       * Initialise the internal FftwAlgorithm time-domain array with zeroes.
       */
      void initFftwArrays();

   private:
      FourierConfig::CSPtr       m_config;
      FftwAlgorithm              m_algorithm;                //! FFTW worker algorithm
      bool                       m_needsInitTimeArr;         //! Bool indicating that the internal FFTW algorithm arrays should be reinitialised to zeroes

   private:
      FourierTransform& operator=( const FourierTransform& other );
      FourierTransform( const FourierTransform& other );
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getConfig
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline const FourierConfig& FourierTransform::getConfig() const
{
   return *m_config;
}


} /// namespace WaveAnalysis

#endif // ADVANCEDFOURIERTRANSFORM_H
