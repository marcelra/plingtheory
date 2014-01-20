#include "WaveletContainer.h"

namespace WaveAnalysis
{

WaveletContainer::WaveletContainer( size_t size, size_t nLayers, size_t zeroPadding, size_t hopOverlap, const SamplingInfo& samplingInfo ) :
   m_maxSize( size ),
   m_nLayers( nLayers ),
   m_zeroPadding( zeroPadding ),
   m_hopOverlap( hopOverlap ),
   m_samplingInfo( samplingInfo ),
   m_spectrumLayers( nLayers ),
   m_windowFunctions( nLayers ),
   m_lowestFrequencies( nLayers )
{}

WaveletContainer::~WaveletContainer()
{
   for ( size_t i = 0; i < m_spectrumLayers.size(); ++i )
   {
      for ( size_t j = 0; j < m_spectrumLayers[i].size(); ++j )
      {
         delete m_spectrumLayers[i][j];
      }
   }
   for ( size_t i = 0; i < m_windowFunctions.size(); ++i )
   {
      delete m_windowFunctions[i];
   }
}

void WaveletContainer::setSpectraForLayer( size_t iLayer, const std::vector< WaveletSpectrum* >& spectra, const WindowFunction& windowFunction )
{
   m_windowFunctions[iLayer] = windowFunction.clone();
   m_spectrumLayers[iLayer] = spectra;
   double numSamplesTimeDomain = m_windowFunctions[iLayer]->getSize();
   double lowestFreq = m_samplingInfo.getSamplingRate() / numSamplesTimeDomain / m_zeroPadding;
   m_lowestFrequencies[iLayer] = lowestFreq;
}

size_t WaveletContainer::getNumLayers() const
{
   return m_spectrumLayers.size();
}

size_t WaveletContainer::getNumSpectraInLayer( size_t index ) const
{
   return m_spectrumLayers[index].size();
}

const WaveletSpectrum& WaveletContainer::getWaveletSpectrum( size_t layerIndex, size_t spectrumIndex ) const
{
   return *m_spectrumLayers[layerIndex][spectrumIndex];
}

double WaveletContainer::getBinFrequency( size_t layerIndex, size_t binIndex ) const
{
   double lowestFreq = m_lowestFrequencies[layerIndex];
   return lowestFreq * binIndex;
}

RealVector WaveletContainer::getBinFrequencies( size_t layerIndex ) const
{
   size_t nBins = m_spectrumLayers[layerIndex][0]->size();
   RealVector result( nBins );
   double lowestFreq = m_lowestFrequencies[0];
   for ( size_t iBin = 0; iBin < nBins; ++iBin )
   {
      result[iBin] = lowestFreq * iBin;
   }
   return result;
}


} /// namespace WaveAnalysis
