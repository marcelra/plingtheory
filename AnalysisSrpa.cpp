#include "AnalysisSrpa.h"

#include "RegularAccumArray.h"
#include "IPlotFactory.h"
#include "Logger.h"
#include "SineGenerator.h"
#include "SpectralReassignmentTransform.h"
#include "SortCache.h"
#include "SrSpecPeakAlgorithm.h"
#include "Utils.h"

namespace Analysis
{

AnalysisSrpa::AnalysisSrpa( const std::string& name, const AlgorithmBase* parent ) :
   AlgorithmBase( name, parent ),
   m_fourierSize( 1024 ),
   m_zeroPadSize( 0 * m_fourierSize ),
   m_frequency( 440 ),
   m_phase( 0 ),
   m_amplitude( 0.1 ),
   m_samplingInfo( 44100 )
{}


/// Correction factor is dependent on number of zero padding samples.
/// Zeropadding = 0: 3.95239, relative uncertainty on amp +- 0.058789
/// Zeropadding = 1: 3.76495
/// Zeropadding = 3: 3.71889

/// Amplitude estimate is independent of phase.
/// The correction factor and relative uncertainty are independent of the amplitude.
/// Min frequency with reasonable error at 1024 samples = 30 Hz
/// Min frequency with reasonable error at 512  samples = 60 Hz

void AnalysisSrpa::studyFrequencyPerformance( const std::vector< double >& frequencies )
{
   Synthesizer::SineGenerator generator( m_samplingInfo );
   FeatureAlgorithm::SrSpecPeakAlgorithm peakAlg( 0.25, "PeakAlg", this );
   WaveAnalysis::SpectralReassignmentTransform transform( m_samplingInfo, m_fourierSize, m_zeroPadSize, 1 );

   RealVector relAmpErrorVec( frequencies.size() );
   RealVector deltaFrequency( frequencies.size() );

   for ( size_t iFreq = 0; iFreq != frequencies.size(); ++iFreq )
   {
      double frequency = frequencies[ iFreq ];

      generator.setFrequency( frequency );
      generator.setAmplitude( m_amplitude );
      generator.setPhase( m_phase );

      RawPcmData::Ptr data = generator.generate( m_fourierSize );
      WaveAnalysis::StftData::Ptr stftData = transform.execute( *data );
      const WaveAnalysis::SrSpectrum& spectrum = stftData->getSrSpectrum( 0 );

      const std::vector< Feature::SrSpecPeak >& peaks = peakAlg.execute( spectrum );

      getLogger() << Msg::Info << "----------------------------------------" << Msg::EndReq;
      getLogger() << Msg::Info << "Truth: frequency = " << frequency << ", amplitude = " << m_amplitude << Msg::EndReq;
      for ( size_t i = 0; i < peaks.size(); ++i )
      {
         double recoAmp = peaks[ i ].getHeight() / m_fourierSize * 3.95239;
         getLogger() << Msg::Info << "Peak " << i << ": frequency = " << peaks[ i ].getFrequency() << ", amplitude " << recoAmp << Msg::EndReq;
         if ( recoAmp > 0.01 )
         {
            double relAmpError = recoAmp / m_amplitude - 1;
            relAmpErrorVec[ iFreq ] = relAmpError;
            deltaFrequency[ iFreq ] = frequency - peaks[ i ].getFrequency();
         }
      }
   }

   double min = Utils::getMinValue( relAmpErrorVec );
   double max = Utils::getMaxValue( relAmpErrorVec );


   Math::RegularAccumArray hist( 50, min - 0.01, max + 0.01 );
   double avgRelAmpError = 0;
   double relAmpError2 = 0;

   for ( size_t i = 0; i < relAmpErrorVec.size(); ++i )
   {
      avgRelAmpError += relAmpErrorVec[ i ];
      relAmpError2 += relAmpErrorVec[ i ] * relAmpErrorVec[ i ];
      hist.add( relAmpErrorVec[ i ], 1 );
   }
   avgRelAmpError /= relAmpErrorVec.size();
   double relAmpVariance = relAmpError2 - avgRelAmpError * avgRelAmpError * relAmpErrorVec.size();
   relAmpVariance /= relAmpErrorVec.size();

   getLogger() << Msg::Info << " --- Statistics --- " << Msg::EndReq;
   getLogger() << Msg::Info << "Average of relative amplitude error = " << avgRelAmpError << Msg::EndReq;
   getLogger() << Msg::Info << "Uncertainty +- " << sqrt( relAmpVariance ) << Msg::EndReq;

   double proposedFactor = avgRelAmpError + 1;
   getLogger() << Msg::Info << "Proposed factor = " << 1 / proposedFactor << Msg::EndReq;



   gPlotFactory().createPlot( "Amp mismatch" );
   gPlotFactory().createHistogram( hist );

   gPlotFactory().createPlot( "RelAmpError vs Frequency" );
   gPlotFactory().createGraph( frequencies, relAmpErrorVec );

   gPlotFactory().createPlot( "Delta frequency vs Frequency" );
   gPlotFactory().createGraph( frequencies, deltaFrequency );
}

} /// namespace Analysis
