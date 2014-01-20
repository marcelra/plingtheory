#include "FourierSpectrum.h"

namespace WaveAnalysis {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor (of array complex)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FourierSpectrum::FourierSpectrum( const SamplingInfo& samplingInfo, const std::complex<double>* first, const std::complex<double>* last ) :
   std::vector< std::complex<double> >( first, last ),
   m_samplingInfo( samplingInfo )
{
   /// Already cast to double since division will be done later
   double numSamples = getNumSamplesTimeDomain();
   assert( numSamples > 0 );
   /// Calculate the lowest frequency in the sample
   m_lowestFreq = getSamplingInfo().getSamplingRate() / numSamples;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FourierSpectrum::~FourierSpectrum()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMagnitude
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVector FourierSpectrum::getMagnitude() const
{
   RealVector result( this->size() );
   for ( size_t i = 0; i < this->size(); ++i )
   {
      result[i] = abs( (*this)[i] );
   }
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getPhase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVector FourierSpectrum::getPhase() const
{
   RealVector result( this->size() );
   for ( size_t i = 0; i < this->size(); ++i )
   {
      result[i] = arg( (*this)[i] );
   }
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getFrequencyOfBin
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double FourierSpectrum::getFrequencyOfBin( size_t binIndex ) const
{
   return m_lowestFreq * binIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getFrequencies
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector< double > FourierSpectrum::getFrequencies() const
{
   std::vector< double > result( size() );
   for ( size_t i = 0; i < size(); ++i )
   {
      result[i] = getFrequencyOfBin( i );
   }
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getFrequency
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double FourierSpectrum::getFrequency( double x ) const
{
   return m_lowestFreq * x;
}

} /// namespace WaveAnalysis
