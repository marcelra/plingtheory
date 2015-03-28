#include "TimeStretcher.h"

#include "IPlotFactory.h"
#include "Logger.h"
#include "PeakSustainAlgorithm.h"
#include "RebinnedSRGraph.h"
#include "SpectralReassignmentTransform.h"
#include "SrSpecPeakAlgorithm.h"
#include "Utils.h"

namespace Music {

TimeStretcher::TimeStretcher( double stretchFactor, const std::string& name, const AlgorithmBase* parent ) :
   AlgorithmBase( name, parent ),
   m_stretchFactor( stretchFactor )
{}

RawPcmData TimeStretcher::execute( const RawPcmData& input )
{
   size_t fourierSize = 2048;
   size_t nZeroPad = 3;
   WaveAnalysis::SpectralReassignmentTransform transform( input.getSamplingInfo(), fourierSize, nZeroPad * fourierSize, 1 );
   const WaveAnalysis::StftData::Ptr& srData = transform.execute( input );
   FeatureAlgorithm::SrSpecPeakAlgorithm peakAlg( 1, this->getName() + "SrSpecPeakAlgorithm", this );

   const WaveAnalysis::FourierConfig& fourierConfig = srData->getConfig();

   std::vector< std::vector< Feature::IBasicSpectrumPeak* > > allPeaks( srData->getNumSpectra() );
   for ( size_t iSpec = 0; iSpec < srData->getNumSpectra(); ++iSpec )
   {
      const WaveAnalysis::SrSpectrum& srSpec = srData->getSrSpectrum( iSpec );

      FeatureAlgorithm::SrSpecPeakAlgorithm::Monitor* monitor = 0;
      if ( iSpec == 2 || iSpec == 1 )
      {
         monitor = new FeatureAlgorithm::SrSpecPeakAlgorithm::Monitor();
      }

      const std::vector< Feature::SrSpecPeak >& peaks = peakAlg.execute( srSpec, monitor );

      std::ostringstream strBuilder;
      strBuilder << "TimeStretcher_" << iSpec;

      if ( monitor )
      {
         monitor->createPeakPlot( strBuilder.str() );
         monitor->createSpectrumPlot( strBuilder.str() );
         monitor->createFrequencyProximityPlot( strBuilder.str() );
      }

      for ( size_t iPeak = 0; iPeak < peaks.size(); ++iPeak )
      {
         allPeaks[ iSpec ].push_back( new Feature::SrSpecPeak( peaks[ iPeak ] ) );
      }
   }

   Visualisation::RebinnedSRGraph graph( *srData );
   graph.create( "testSpectralReassignment/SrGraph" );

   FeatureAlgorithm::PeakSustainAlgorithm sustainedPeakAlg( "TimeStretcher.PeakSustainAlgorithm", this );
   std::vector< Feature::SustainedPeak* > sustainedPeaks = sustainedPeakAlg.execute( allPeaks );

   for ( size_t iPeak = 0; iPeak < allPeaks.size(); ++iPeak )
   {
      for ( size_t jPeak = 0; jPeak < allPeaks[ iPeak ].size(); ++jPeak )
      {
         const Feature::IBasicSpectrumPeak& peak = *allPeaks[ iPeak ][ jPeak ];
         // getLogger() << Msg::Verbose << "peak time values: " << peak.getStartTimeSamples() << ", " << peak.getEndTimeSamples() << Msg::EndReq;
         gPlotFactory().createGraph( realVector( peak.getStartTimeSamples(), peak.getEndTimeSamples() ), realVector( peak.getFrequency(), peak.getFrequency() ), Qt::yellow );
      }
   }

   for ( size_t iSustPeak = 0; iSustPeak < sustainedPeaks.size(); ++iSustPeak )
   {
      const Feature::SustainedPeak& peak = *sustainedPeaks[ iSustPeak ];
      gPlotFactory().createGraph( realVector( peak.getStartTimeSamples(), peak.getEndTimeSamples() ), realVector( peak.getFrequency(), peak.getFrequency() ), Qt::white );
   }

   size_t originalLength = input.size();
   const SamplingInfo& samplingInfo = input.getSamplingInfo();

   const RawPcmData& result = generateFromSustainedPeaks( sustainedPeaks, fourierConfig, samplingInfo, originalLength );

   for ( size_t i = 0; i < allPeaks.size(); ++i )
   {
      Utils::cleanupVector( allPeaks[ i ] );
   }
   Utils::cleanupVector( sustainedPeaks );


   return result;

}

RawPcmData TimeStretcher::generateFromSustainedPeaks( const std::vector< Feature::SustainedPeak* >& sustainedPeaks,
                                                      const WaveAnalysis::FourierConfig& fourierConfig,
                                                      const SamplingInfo& samplingInfo,
                                                      size_t originalLength ) const
{
   size_t newLength = ceil( ( originalLength + fourierConfig.getWindowSize() ) * m_stretchFactor );
   RawPcmData result( samplingInfo, newLength );

   double windowFunctionIntegral = 0;
   for ( size_t i = 0; i < fourierConfig.getWindowSize(); ++i )
   {
      windowFunctionIntegral += fourierConfig.getWindowFunction().calc( i );
   }
   windowFunctionIntegral /= fourierConfig.getWindowSize();

   getLogger() << Msg::Debug << "Window function integral = " << windowFunctionIntegral << Msg::EndReq;

   double normFactor = 1 / sqrt( fourierConfig.getWindowSize() );
   normFactor = normFactor * normFactor / windowFunctionIntegral / 2;

   size_t decayLength = 256;

   for ( size_t iPeak = 0; iPeak < sustainedPeaks.size(); ++iPeak )
   {
      const Feature::SustainedPeak& peak = *sustainedPeaks[ iPeak ];
      double phase = M_PI * ( iPeak % 2 );
      size_t startSample = m_stretchFactor * peak.getStartTimeSamples();
      size_t endSample = m_stretchFactor * peak.getEndTimeSamples();
      double frequency = peak.getFrequency();
      double amplitude0 = peak.getHeight() * normFactor;
      double amplitude = amplitude0;
      double ampDecay = amplitude0 / decayLength;

      double phaseStep = samplingInfo.getPhaseStepPerSample( frequency );

      for ( size_t iSample = startSample; iSample < endSample; ++iSample, phase += phaseStep )
      {
         double val = amplitude * sin( phase );
         // getLogger() << Msg::Verbose << "val = " << val << Msg::EndReq;
         result[ iSample ] += val;
         if ( ( endSample - iSample ) < decayLength )
         {
            amplitude = amplitude - ampDecay;
         }
      }

   }

   return result;
}




} /// namespace Music
