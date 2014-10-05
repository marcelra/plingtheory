#include "AnalysisSrpa.h"

#include "IPlotFactory.h"
#include "Logger.h"
#include "SineGenerator.h"
#include "SpectralReassignmentTransform.h"
#include "SortCache.h"
#include "SrSpecPeakAlgorithm.h"
#include "Utils.h"

namespace Analysis
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AnalysisSrpa::AnalysisSrpa( const std::string& name, const AlgorithmBase* parent ) :
   AlgorithmBase( name, parent ),
   m_fourierSize( 1024 ),
   m_zeroPadSize( 0 * m_fourierSize ),
   m_amplitude( 1 ),
   m_samplingInfo( 44100 )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setFourierConfig
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AnalysisSrpa::setFourierConfig( size_t fourierSize, size_t numSamplesZeroPadding )
{
   m_fourierSize = fourierSize;
   m_zeroPadSize = numSamplesZeroPadding;
}

/// Correction factor is dependent on number of zero padding samples.
/// The correction factor for zeropadding = 0 has been added to the SrSpecPeakAlgorithm
/// Zeropadding = 0: 3.95239, relative uncertainty on amp +- 0.058789
/// Zeropadding = 1: 3.76495
/// Zeropadding = 3: 3.71889

/// Amplitude estimate is independent of phase.
/// The correction factor and relative uncertainty are independent of the amplitude.
/// Min frequency with reasonable error at 1024 samples = 30 Hz
/// Min frequency with reasonable error at 512  samples = 60 Hz

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// studyFrequencyPerformance
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

      RawPcmData::Ptr data = generator.generate( m_fourierSize );
      WaveAnalysis::StftData::Ptr stftData = transform.execute( *data );
      const WaveAnalysis::SrSpectrum& spectrum = stftData->getSrSpectrum( 0 );

      const std::vector< Feature::SrSpecPeak >& peaks = peakAlg.execute( spectrum );

      getLogger() << Msg::Info << "----------------------------------------" << Msg::EndReq;
      getLogger() << Msg::Info << "Truth: frequency = " << frequency << ", amplitude = " << m_amplitude << Msg::EndReq;
      for ( size_t i = 0; i < peaks.size(); ++i )
      {
         double recoAmp = peaks[ i ].getHeight() / m_fourierSize;
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



   gPlotFactory().createPlot( getName() + "/Amp mismatch" );
   gPlotFactory().createHistogram( hist );

   gPlotFactory().createPlot( getName() + "/RelAmpError vs Frequency" );
   gPlotFactory().createGraph( frequencies, relAmpErrorVec );

   gPlotFactory().createPlot( getName() + "/Delta frequency vs Frequency" );
   gPlotFactory().createGraph( frequencies, deltaFrequency );
}

/// Quality classification enumeration of twin peak detection.
enum HistStatus
{
   NotEnoughPeaks = 0,
   PeaksIncorrect,
   PeaksCorrect,
   NumHistStatusItems
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// studyTwinPeakPerformance
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AnalysisSrpa::studyTwinPeakPerformance( const std::vector< double >& frequencies, const std::vector< double >& frequencyDifference, double amp2 )
{
   Synthesizer::SineGenerator generator1( m_samplingInfo );
   Synthesizer::SineGenerator generator2( m_samplingInfo );
   FeatureAlgorithm::SrSpecPeakAlgorithm peakAlg( 0.25, "PeakAlg", this );
   WaveAnalysis::SpectralReassignmentTransform transform( m_samplingInfo, m_fourierSize, m_zeroPadSize, 1 );

   generator1.setAmplitude( m_amplitude );
   generator2.setAmplitude( amp2 );

   Math::RegularAccumArray statusHist( 3, -0.5, NumHistStatusItems - 0.5 );

   /// Loop over frequencies.
   for ( size_t iFreq = 0; iFreq < frequencies.size(); ++iFreq )
   {
      double frequency = frequencies[ iFreq ];
      generator1.setFrequency( frequency );
      RawPcmData::Ptr data1 = generator1.generate( m_fourierSize );

      RealVector freqDiffX;
      RealVector freqDiffYMin;
      RealVector freqDiffYMax;

      /// Loop over frequency differences.
      for ( size_t iFreqDiff = 0; iFreqDiff < frequencyDifference.size(); ++iFreqDiff )
      {
         double frequency2 = generator1.getFrequency() + frequencyDifference[ iFreqDiff ];
         generator2.setFrequency( frequency2 );
         RawPcmData::Ptr data2 = generator2.generate( m_fourierSize );

         RawPcmData data( *data1 );
         data.mixAdd( *data2 );

         WaveAnalysis::StftData::Ptr stftData = transform.execute( data );
         const WaveAnalysis::SrSpectrum& spectrum = stftData->getSrSpectrum( 0 );

         FeatureAlgorithm::SrSpecPeakAlgorithm::Monitor* monitor = 0;
         if ( iFreq == 0 && iFreqDiff == 2 )
         {
            monitor = new FeatureAlgorithm::SrSpecPeakAlgorithm::Monitor();
         }

         const std::vector< Feature::SrSpecPeak >& peaks = peakAlg.execute( spectrum, monitor );

         if ( monitor )
         {
            monitor->createSpectrumPlot( getName() + "/FreqProx/FreqMonitor" );
            delete monitor;
         }

         getLogger() << Msg::Info << "----------------------------------------" << Msg::EndReq;
         getLogger() << Msg::Info << "iFreq = " << iFreq << ", iFreqDiff = " << iFreqDiff << Msg::EndReq;
         getLogger() << Msg::Info << "Truth sinusoid 1: frequency = " << generator1.getFrequency() << ", amplitude = " << generator1.getAmplitude() << Msg::EndReq;
         getLogger() << Msg::Info << "Truth sinusoid 2: frequency = " << generator2.getFrequency() << ", amplitude = " << generator2.getAmplitude() << Msg::EndReq;

         RealVector peakAmps( peaks.size() );
         for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
         {
            peakAmps[ iPeak ] = peaks[ iPeak ].getHeight();
         }

         SortCache peakAmpsSc( peakAmps );
         if ( peakAmpsSc.getSize() >= 2 )
         {
            double freqEstimate1 = peaks[ peakAmpsSc.getReverseSortedIndex( 0 ) ].getFrequency();
            double freqEstimate2 = peaks[ peakAmpsSc.getReverseSortedIndex( 1 ) ].getFrequency();

            double errDf1;
            double errDf2;

            int estimate1 = 2;
            int estimate2 = 2;
            if ( fabs( freqEstimate1 - frequency ) < fabs( freqEstimate1 - frequency2 ) )
            {
               estimate1 = 1;
               errDf1 = freqEstimate1 - frequency;
               errDf2 = freqEstimate2 - frequency2;
            }
            if ( fabs( freqEstimate2 - frequency ) < fabs( freqEstimate2 - frequency2 ) )
            {
               estimate2 = 1;
               errDf1 = freqEstimate2 - frequency;
               errDf2 = freqEstimate1 - frequency2;
            }

            if ( estimate1 != estimate2 )
            {
               statusHist.add( PeaksCorrect, 1 );
               freqDiffX.push_back( frequencyDifference[ iFreqDiff ] );
               freqDiffYMin.push_back( std::min( errDf1, errDf2 ) );
               freqDiffYMax.push_back( std::max( errDf1, errDf2 ) );
            }
            else
            {
               statusHist.add( PeaksIncorrect, 1 );
            }
         }
         else
         {
            statusHist.add( NotEnoughPeaks, 1 );
         }

         for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
         {
            double recoAmp = peaks[ iPeak ].getHeight() / m_fourierSize;
            getLogger() << Msg::Info << "Peak " << iPeak << ": frequency = " << peaks[ iPeak ].getFrequency() << ", amplitude " << recoAmp << Msg::EndReq;
         }

      } /// Loop over frequency differences.

      std::ostringstream plotTitleFreqDifMin;
      plotTitleFreqDifMin << getName() + "/FreqProx/FreqError" << frequency;
      gPlotFactory().createPlot( plotTitleFreqDifMin.str() );
      gPlotFactory().createGraph( freqDiffX, freqDiffYMin, Qt::blue );
      gPlotFactory().createGraph( freqDiffX, freqDiffYMax, Qt::red );

   } /// Loop over frequencies.

   gPlotFactory().createPlot( getName() + "/FroxProx/CategorisedCounts" );
   gPlotFactory().createHistogram( statusHist );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// studyAmpBehaviour
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AnalysisSrpa::studyAmpBehaviour( double frequency, double ampFracBegin, double ampFracEnd )
{
   SamplingInfo samplingInfo;
   FeatureAlgorithm::SrSpecPeakAlgorithm peakAlg( 0.25, "PeakAlg", this );
   WaveAnalysis::SpectralReassignmentTransform transform( m_samplingInfo, m_fourierSize, m_zeroPadSize, 1 );

   RawPcmData data( samplingInfo, m_fourierSize );

   double ampStart = m_amplitude * ampFracBegin;
   double ampEnd = m_amplitude * ampFracEnd;

   double ampStep = ( ampStart- ampEnd ) / data.size();
   double amp = ampStart;

   double phase = 0;
   double phaseStep = samplingInfo.getPhaseStepPerSample( frequency );

   for ( size_t i = 0; i < data.size(); ++i )
   {
      data[ i ] = sin( phase ) * amp;
      phase += phaseStep;
      amp -= ampStep;
   }

   gPlotFactory().createPlot( getName() + "/AmpBehaviour/Data" );
   gPlotFactory().createGraph( *data.copyToVectorData() );

   WaveAnalysis::StftData::Ptr stftData = transform.execute( data );
   const WaveAnalysis::SrSpectrum& spectrum = stftData->getSrSpectrum( 0 );

   FeatureAlgorithm::SrSpecPeakAlgorithm::Monitor* monitor = 0;
   monitor = new FeatureAlgorithm::SrSpecPeakAlgorithm::Monitor();

   const std::vector< Feature::SrSpecPeak >& peaks = peakAlg.execute( spectrum, monitor );
   /// Prevent compiler warning.
   assert( &peaks );

   if ( monitor )
   {
      monitor->createSpectrumPlot( getName() + "/AmpBehaviour/" );
      delete monitor;
   }

   gPlotFactory().createScatter( spectrum.getFrequencies(), spectrum.getTimeCorrections() * 0.01 );

}

} /// namespace Analysis
