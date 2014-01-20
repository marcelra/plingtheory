#ifndef WAVELETCONTAINER_H
#define WAVELETCONTAINER_H

#include "SamplingInfo.h"
#include "WaveletSpectrum.h"

namespace WaveAnalysis
{

class WaveletContainer
{
   public:
      WaveletContainer( size_t size, size_t nLayers, size_t zeroPaadding, size_t hopOverlap, const SamplingInfo& samplingInfo );
      virtual ~WaveletContainer();

      void setSpectraForLayer( size_t iLayer, const std::vector< WaveletSpectrum* >& spectra, const WindowFunction& windowFunction );

   public:
      size_t getNumLayers() const;
      size_t getNumSpectraInLayer( size_t layerIndex ) const;
      const WaveletSpectrum& getWaveletSpectrum( size_t layerIndex, size_t spectrumIndex ) const;

      double getBinFrequency( size_t layerIndex, size_t binIndex ) const;
      RealVector getBinFrequencies( size_t layerIndex ) const;

   private:
      size_t       m_maxSize;
      size_t       m_nLayers;
      size_t       m_zeroPadding;
      size_t       m_hopOverlap;
      SamplingInfo m_samplingInfo;

      std::vector< std::vector< WaveletSpectrum* > >   m_spectrumLayers;
      std::vector< const WindowFunction* >             m_windowFunctions;
      std::vector< double >                            m_lowestFrequencies;
};

} /// WaveAnalysis

#endif // WAVELETCONTAINER_H
