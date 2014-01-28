#include "FourierConfig.h"

#include "FftwAlgorithm.h"
#include "SamplingInfo.h"

namespace WaveAnalysis
{

FourierConfig::FourierConfig( const SamplingInfo& samplingInfo, size_t windowSize, const WindowFuncDef& windowFuncDef, size_t numSamplesZeroPadding ) :
   m_samplingInfo( samplingInfo ),
   m_windowSize( windowSize ),
   m_windowFuncDef( windowFuncDef.clone() ),
   m_windowFunction( windowFuncDef.createWindowFunction( windowSize ) ),
   m_numSamplesZeroPadding( numSamplesZeroPadding )
{
   initFrequencyList();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// initFrequencyList
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FourierConfig::initFrequencyList()
{
   size_t numFrequencies = getSpectrumDimension();
   m_frequencies.resize( numFrequencies );
   double lowestFrequency = getLowestFrequency();
   for ( size_t i = 0; i < numFrequencies; ++i )
   {
      m_frequencies[i] = lowestFrequency * i;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getSpectrumDimension
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t FourierConfig::getSpectrumDimension() const
{
   return FftwAlgorithm::getSpectrumDimension( getTotalFourierSize() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getLowestFrequency
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double FourierConfig::getLowestFrequency() const
{
   return m_samplingInfo.getSamplingRate() / getTotalFourierSize();
}


} /// namespace WaveAnalysis
