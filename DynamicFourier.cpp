#include "DynamicFourier.h"

#include <iostream>

namespace WaveAnalysis
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DynamicFourier::DynamicFourier( const std::vector< double >& testFrequencies, double nPeriods ) :
   m_testFrequencies( testFrequencies ),
   m_nPeriods( nPeriods )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Destrctor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DynamicFourier::~DynamicFourier()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// execute
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ResonanceMatrix DynamicFourier::execute( const RawPcmData& data ) const
{
   ResonanceMatrix result( m_testFrequencies.size(), std::vector< double >( data.size() ) );

   for ( size_t iFreq = 0; iFreq < m_testFrequencies.size(); ++iFreq )
   {
      double frequency = m_testFrequencies[iFreq];
      std::cout << "Calculating spectrum for frequency: " << frequency << std::endl;
      const std::vector< Complex >& integrand = getIntegrand( data, frequency );

      double period = data.getSamplingInfo().getPeriodInSamples( frequency );
      size_t nSamplesIntegrate = period*m_nPeriods;

      const std::vector< Complex >& transform = integrate( integrand, nSamplesIntegrate );

      assert( transform.size() == data.size() );

      std::vector< double >& ampOverTime = result[iFreq];
      size_t nSamplesPeriod = data.getSamplingInfo().getPeriodInSamples( frequency );
      // size_t nSamplesPeriod = 0;
      for ( size_t i = 0; i < transform.size() - nSamplesPeriod; ++i )
      {
         // ampOverTime[i] = abs( transform[i] );
         double currentPhase = arg( transform[i] );
         double futurePhase = arg( transform[i + nSamplesPeriod ] );
         // double sinPhaseDiff = sin( currentPhase - prevPhase );
         double phaseDiff = sin( futurePhase - currentPhase );
         // ampOverTime[i] = prevPhase - currentPhase;  // 1 / ( 0.0001 + sinPhaseDiff*sinPhaseDiff );
         ampOverTime[i] = 1 / ( 0.1 + phaseDiff*phaseDiff ) * abs( transform[i] );
         // std::cout << currentPhase - prevPhase << std::endl;

         // prevPhase = currentPhase;
      }
   }

   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getIntegrand
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector< DynamicFourier::Complex > DynamicFourier::getIntegrand( const RawPcmData& data, double frequency ) const
{
   double phaseAdvance = data.getSamplingInfo().getPhaseStepPerSample( frequency );
   double phase = 0;

   std::vector< Complex > result( data.size() );

   for ( size_t iSample = 0; iSample < data.size(); ++iSample )
   {
      result[iSample].real() = data[iSample]*cos( phase )*frequency;
      result[iSample].imag() = data[iSample]*sin( phase )*frequency;
      phase += phaseAdvance;
   }

   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// integrate
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector< DynamicFourier::Complex > DynamicFourier::integrate( const std::vector< Complex >& integrand, size_t nSamplesInt ) const
{
   std::vector< Complex > result( integrand.size() );

   Complex z( 0, 0 );

   /// Get initial integral
   for ( size_t i = 0; i < nSamplesInt; ++i )
   {
      z.real() += integrand[i].real();
      z.imag() += integrand[i].imag();
   }
   result[0] = z;

   for ( size_t iSample = 1; iSample < integrand.size() - nSamplesInt; ++iSample )
   {
      result[iSample].real() = result[iSample-1].real() - integrand[iSample-1].real() + integrand[iSample + nSamplesInt -1].real();
      result[iSample].imag() = result[iSample-1].imag() - integrand[iSample-1].imag() + integrand[iSample + nSamplesInt -1].imag();
   }

   for ( size_t iSample = integrand.size() - nSamplesInt; iSample < integrand.size(); ++iSample )
   {
      result[iSample].real() = result[iSample-1].real() - integrand[iSample-1].real();
      result[iSample].imag() = result[iSample-1].imag() - integrand[iSample-1].imag();
   }

   return result;
}


} /// namespace WaveAnalysis
