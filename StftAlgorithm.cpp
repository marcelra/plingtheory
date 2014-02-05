#include "StftAlgorithm.h"

#include <iostream>

namespace WaveAnalysis
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StftAlgorithm::StftAlgorithm( const SamplingInfo& samplingInfo, size_t windowSize, const WindowFuncDef& windowFuncDef, size_t numSamplesZeroPadding, double hopsPerWindow ):
   m_hopsPerWindow( hopsPerWindow ),
   m_transform( samplingInfo, windowSize, windowFuncDef, numSamplesZeroPadding )
{
   assert( hopsPerWindow >= 1 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// execute
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RawStftData::Ptr StftAlgorithm::execute( const RawPcmData& data )
{
   assert( m_transform.getConfig().getSamplingInfo() == data.getSamplingInfo() );
   assert( getHopShift() > 1 );

   RawStftData* result = new RawStftData( m_transform.getConfigCSPtr() );

   double numHopsD = data.size() / getHopShift();
   double currentSampleD = 0;
   size_t numHops = numHopsD + 1;
   for ( size_t iHop = 0; iHop < numHops; ++iHop )
   {
      size_t currentSampleI = currentSampleD;
      RawStftData::WindowLocation* windowLocation = new RawStftData::WindowLocation( currentSampleI, currentSampleI + m_transform.getConfig().getWindowSize() );

      if ( currentSampleI + m_transform.getConfig().getWindowSize() < data.size() )
      {
         const double* timeData = &(data[currentSampleI]);
         result->addSpectrum( m_transform.transform( timeData ).release(), windowLocation );
      }
      else
      {
         std::cout << "Enlarging data set at position " << currentSampleI << std::endl;
         const RealVector& extendedVector = extendDataWithZeros( data, currentSampleI );
         result->addSpectrum( m_transform.transform( &extendedVector[0] ).release(), windowLocation );
      }
      currentSampleD += getHopShift();
   }

   return RawStftData::Ptr( result );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// reverseExecute
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RawPcmData::Ptr StftAlgorithm::reverseExecute( const RawStftData& stftData )
{
   assert( m_transform.getConfig().isInvertible() );

   /// Create RawPcmData object
   const SamplingInfo& samplingInfo = m_transform.getConfig().getSamplingInfo();
   size_t numSamplesData = stftData.getWindowLocationNoOverlap( stftData.getNumSpectra() - 1 ).getLastSample();
   RawPcmData* result = new RawPcmData( samplingInfo, numSamplesData );

   for ( size_t iSpec = 0; iSpec < stftData.getNumSpectra(); ++iSpec )
   {
      RealVectorPtr vec = m_transform.transform( stftData.getSpectrum( iSpec ) );
      const RawStftData::WindowLocation& winLocNoOverlap = stftData.getWindowLocationNoOverlap( iSpec );
      size_t vecIndex = stftData.getWindowLocationNoOverlap( iSpec ).getFirstSample() - stftData.getWindowLocation( iSpec ).getFirstSample();
      for ( size_t iSample = winLocNoOverlap.getFirstSample(); iSample <= winLocNoOverlap.getLastSample(); ++iSample, ++vecIndex )
      {
         (*result)[ iSample ] = (*vec)[ vecIndex ];
      }
   }

   return RawPcmData::Ptr( result );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// (internal) extendDataWithZero
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVector StftAlgorithm::extendDataWithZeros( const RawPcmData& data, size_t currentSample )
{
   RealVector result( data.begin() + currentSample, data.end() );
   result.resize( m_transform.getConfig().getWindowSize(), 0 );
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getHopShift
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double StftAlgorithm::getHopShift() const
{
   return m_transform.getConfig().getWindowSize() / m_hopsPerWindow;
}

size_t StftAlgorithm::getSpectrumDimension() const
{
   return m_transform.getConfig().getSpectrumDimension();
}

FourierConfig::CSPtr StftAlgorithm::getConfig() const
{
   return m_transform.getConfigCSPtr();
}

} /// namespace WaveAnalysis
