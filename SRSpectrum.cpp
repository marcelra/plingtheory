#include "SRSpectrum.h"

namespace WaveAnalysis
{

SRSpectrum::SRSpectrum( const FourierSpectrum& ft, const FourierSpectrum& ftDerivative, const FourierSpectrum& ftTimeRamp ) :
   FourierSpectrum( ft ),
   m_correctedFrequencies( getConfig().getSpectrumDimension() ),
   m_freqCorrections( getConfig().getSpectrumDimension() ),
   m_timeCorrections( getConfig().getSpectrumDimension() )
{
   assert( getConfig() == ft.getConfig() );

   for ( size_t i = 0; i < ft.size(); ++i )
   {
      double normInv = 1. / norm( ft[i] );
      m_freqCorrections[i] = normInv * ( ftDerivative[i] * conj( ft[i] ) ).imag() * ft.getConfig().getSamplingInfo().getSamplingRate() / 2. / M_PI;
      m_timeCorrections[i] = normInv * ( ftTimeRamp[i] * conj( ft[i] ) ).real();
      m_correctedFrequencies[i] = getConfig().getSpectrumFrequencies()[i] + m_freqCorrections[i];
   }
}

double SRSpectrum::getFrequencyOfBin( size_t binIndex ) const
{
   return m_correctedFrequencies[ binIndex ];
}

const RealVector& SRSpectrum::getFrequencies() const
{
   return m_correctedFrequencies;
}

double SRSpectrum::getFrequency( double x ) const
{
   size_t indexLeft = floor( x );
   size_t indexRight = ceil( x );
   double diff = m_correctedFrequencies[ indexRight ] - m_correctedFrequencies[ indexLeft ];
   return m_correctedFrequencies[ indexLeft ] + ( x - indexLeft ) * diff;
}

const RealVector& SRSpectrum::getFrequencyCorrections() const
{
   return m_freqCorrections;
}

const RealVector& SRSpectrum::getTimeCorrections() const
{
   return m_timeCorrections;
}

} /// namespace WaveAnalysis
