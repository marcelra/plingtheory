#include "FourierFilter.h"

#include "RootUtilities.h"

#include <math.h>

namespace WaveAnalysis {

FourierFilter::FourierFilter( double frequency, size_t nPeriods, const SamplingInfo& samplingInfo ) :
   m_frequency( frequency ),
   m_nPeriods( nPeriods ),
   m_samplingInfo( samplingInfo )
{
   m_nSamplesBase = m_nPeriods * m_samplingInfo.getPeriodInSamples( m_frequency );
}

RawPcmData::Ptr FourierFilter::apply( const RawPcmData& data ) const
{
   Logger msg( "FourierFilter" );
   msg << Msg::Info << "Applying the fourier transform to a data set with " << data.size() << " samples." << Msg::EndReq;

   assert( data.getSamplingInfo() == m_samplingInfo );
   assert( data.size() > m_nSamplesBase );

   RawPcmData& output = *(new RawPcmData( m_samplingInfo, data.size() ) );

   std::vector<double> transformedDataOdd( data.size() );
   std::vector<double> transformedDataEven( data.size() );

   double phase = 0;
   double phaseStep = m_samplingInfo.getPhaseStepPerSample( m_frequency );

   for ( size_t iSample = 0; iSample < data.size(); ++iSample )
   {
      transformedDataOdd[iSample] = data[iSample]*sin( phase ) / m_nSamplesBase;
      transformedDataEven[iSample] = data[iSample]*cos( phase ) / m_nSamplesBase;
      phase += phaseStep;
   }

   //new TCanvas();
   //RootUtilities::createGraph( transformedData )->Draw( "AL" );

   msg << Msg::Info << "Partially integrating the transformed results..." << Msg::EndReq;
   // std::vector<double> integralEven( data.size() );
   // std::vector<double> integralOdd( data.size() );

   double prevIntOdd = transformedDataOdd[0];
   double prevIntEven = transformedDataEven[0];
   double intOdd = transformedDataOdd[0];
   double intEven = transformedDataEven[0];
   // integralOdd[0] = transformedDataOdd[0];
   // integralEven[0] = transformedDataEven[0];
   for ( size_t iSample = 1; iSample < m_nSamplesBase; ++iSample )
   {
      intOdd = prevIntOdd + transformedDataOdd[iSample];
      intEven = prevIntEven + transformedDataEven[iSample];
      output[iSample] = intOdd*intOdd + intEven*intEven;
      prevIntOdd = intOdd;
      prevIntEven = intEven;
   }

   for ( size_t iSample = m_nSamplesBase; iSample < data.size(); ++iSample )
   {
      // output[iSample] = output[iSample - 1] - transformedData[iSample - m_nSamplesBase] + transformedData[iSample];
      intOdd = prevIntOdd - transformedDataOdd[iSample - m_nSamplesBase] + transformedDataOdd[iSample];
      intEven = prevIntEven - transformedDataEven[iSample - m_nSamplesBase] + transformedDataEven[iSample];
      output[iSample] = sqrt(intOdd*intOdd + intEven*intEven);
      prevIntOdd = intOdd;
      prevIntEven = intEven;
   }

   msg << Msg::Info << "Fourier transform done." << Msg::EndReq;

   return RawPcmData::Ptr( &output );
}

} /// namespace WaveAnalysis
