#include "StftAlgorithm.h"

#include <iostream>

namespace WaveAnalysis
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StftAlgorithm::StftAlgorithm( const SamplingInfo& samplingInfo, size_t windowSize, const WindowFuncDef& windowFuncDef, size_t numSamplesZeroPadding, double hopsPerWindow ):
   m_hasExecuted( false ),
   m_hopsPerWindow( hopsPerWindow ),
   m_transform( samplingInfo, windowSize, windowFuncDef, numSamplesZeroPadding )
{
   assert( hopsPerWindow >= 1 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StftAlgorithm::~StftAlgorithm()
{
   for ( size_t i = 0; i < m_transformedData.size(); ++i )
   {
      delete m_transformedData[i];
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// execute
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void StftAlgorithm::execute( const RawPcmData& data )
{
   assert( !m_hasExecuted );
   assert( m_transform.getConfig().getSamplingInfo() == data.getSamplingInfo() );
   assert( getHopShift() > 1 );

   double numHopsD = data.size() / getHopShift();
   double currentSampleD = 0;
   size_t numHops = numHopsD + 1;
   for ( size_t iHop = 0; iHop < numHops; ++iHop )
   {
      size_t currentSampleI = currentSampleD;
      WindowLocation windowLocation( currentSampleI, currentSampleI + m_transform.getConfig().getWindowSize() );
      m_windowLocations.push_back( windowLocation );

      if ( currentSampleI + m_transform.getConfig().getWindowSize() < data.size() )
      {
         const double* timeData = &(data[currentSampleI]);
         m_transformedData.push_back( m_transform.transform( timeData ).release() );
      }
      else
      {
         std::cout << "Enlarging data set at position " << currentSampleI << std::endl;
         const RealVector& extendedVector = extendDataWithZeros( data, currentSampleI );
         m_transformedData.push_back( m_transform.transform( &extendedVector[0] ).release() );
      }
      currentSampleD += getHopShift();
   }

   m_hasExecuted = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// reverseExecute
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RawPcmData::Ptr StftAlgorithm::reverseExecute()
{
   assert( m_transform.getConfig().isInvertible() );

   /// Create RawPcmData object
   const SamplingInfo& samplingInfo = m_transform.getConfig().getSamplingInfo();
   size_t numSamplesData = getWindowLocationNoOverlap( getNumSpectra() - 1 ).getLastSample();
   RawPcmData* result = new RawPcmData( samplingInfo, numSamplesData );

   for ( size_t iSpec = 0; iSpec < getNumSpectra(); ++iSpec )
   {
      RealVectorPtr vec = m_transform.transform( getSpectrum( iSpec ) );
      const WindowLocation& winLocNoOverlap = getWindowLocationNoOverlap( iSpec );
      size_t vecIndex = getWindowLocationNoOverlap( iSpec ).getFirstSample() - getWindowLocation( iSpec ).getFirstSample();
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
/// getNumSpectra
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t StftAlgorithm::getNumSpectra() const
{
   return m_transformedData.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getSpectrum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FourierSpectrum& StftAlgorithm::getSpectrum( size_t spectrumIndex )
{
   return *m_transformedData[spectrumIndex];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getSpectrum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const FourierSpectrum& StftAlgorithm::getSpectrum( size_t spectrumIndex ) const
{
   return *m_transformedData[spectrumIndex];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getClosestSpectrumAt
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FourierSpectrum& StftAlgorithm::getClosestSpectrumAt( size_t sampleIndex )
{
   return const_cast< FourierSpectrum& >( const_cast< const StftAlgorithm* >( this )->getClosestSpectrumAt( sampleIndex ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getClosestSpectrumAt
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const FourierSpectrum& StftAlgorithm::getClosestSpectrumAt( size_t sampleIndex ) const
{
   size_t spectrumIndex = sampleIndex / getHopShift();
   assert( spectrumIndex < m_transformedData.size() );

   return *m_transformedData[ spectrumIndex ];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getHopShift
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double StftAlgorithm::getHopShift() const
{
   return m_transform.getConfig().getWindowSize() / m_hopsPerWindow;
}

const StftAlgorithm::WindowLocation& StftAlgorithm::getWindowLocation( size_t spectrumIndex ) const
{
   return m_windowLocations[ spectrumIndex ];
}

StftAlgorithm::WindowLocation StftAlgorithm::getWindowLocationNoOverlap( size_t spectrumIndex ) const
{
   const WindowLocation& winLocCentre = getWindowLocation( spectrumIndex );
   size_t centreCentre = winLocCentre.getCentre();

   /// Boundary cases
   size_t resultLeft = 0;
   size_t resultRight = getWindowLocation( getNumSpectra() - 1 ).getLastSample();
   size_t centreRight = winLocCentre.getLastSample();

   if ( spectrumIndex > 0 )
   {
      const WindowLocation& winLocLeft = getWindowLocation( spectrumIndex - 1 );
      size_t centreLeft = winLocLeft.getCentre();
      resultLeft = ( centreLeft + centreCentre ) / 2;
   }
   if ( spectrumIndex < getNumSpectra() - 1 )
   {
      const WindowLocation& winLocRight = getWindowLocation( spectrumIndex + 1 );
      size_t centreRight = winLocRight.getCentre();
      resultRight = ( centreCentre + centreRight ) / 2;
   }

   if ( resultLeft < winLocCentre.getFirstSample() )
   {
      resultLeft = winLocCentre.getFirstSample();
   }
   if ( centreRight > winLocCentre.getLastSample() )
   {
      resultRight = winLocCentre.getLastSample();
   }

   WindowLocation result( resultLeft, resultRight - 1 );
   return result;
}

size_t StftAlgorithm::getSpectrumDimension() const
{
   return m_transform.getConfig().getSpectrumDimension();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// WindowLocation subclass
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StftAlgorithm::WindowLocation::WindowLocation( size_t firstSampleIndex, size_t lastSampleIndex ) :
   m_first( firstSampleIndex ),
   m_last( lastSampleIndex )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getFirstSample
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t StftAlgorithm::WindowLocation::getFirstSample() const
{
   return m_first;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getLastSample
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t StftAlgorithm::WindowLocation::getLastSample() const
{
   return m_last;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getCentre
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t StftAlgorithm::WindowLocation::getCentre() const
{
   return ( m_first + m_last ) / 2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// hasExecuted
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool StftAlgorithm::hasExecuted() const
{
   return m_hasExecuted;
}

} /// namespace WaveAnalysis
