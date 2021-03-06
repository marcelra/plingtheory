#include "SrSpecPeakAlgorithm.h"

#include "IPlotFactory.h"
#include "LinearInterpolator.h"
#include "Logger.h"
#include "Plot2D.h"
#include "PredefinedRealFunctions.h"
#include "SampledMovingAverage.h"
#include "SortCache.h"
#include "SrSpectrum.h"
#include "Utils.h"
#include "WindowLocation.h"

#include <limits>

namespace Feature
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SrSpecPeak methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SrSpecPeak::SrSpecPeak( double frequency, double height, double frequencyUncertainty, size_t startTimeSamples, size_t endTimeSamples ) :
   m_frequency( frequency ),
   m_height( height ),
   m_freqUnc( frequencyUncertainty ),
   m_startTimeSamples( startTimeSamples ),
   m_endTimeSamples( endTimeSamples )
{}

} /// namespace Feature



namespace FeatureAlgorithm
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SrSpecPeakAlgorithm::Monitor methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SrSpecPeakAlgorithm::Monitor::Monitor() :
   baseline( 0 )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SrSpecPeakAlgorithm::Monitor::~Monitor()
{
   delete baseline;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// createSpectrumPlot
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SrSpecPeakAlgorithm::Monitor::createSpectrumPlot( const std::string& prefix )
{
   Math::Log10Function tr;

   Plotting::Plot2D& plot = gPlotFactory().createPlot( prefix + "SpectrumPlot" );
   plot.setXAxisTitle( "Frequency (Hz)" );
   plot.setYAxisTitle( "10log Amplitude" );
   gPlotFactory().createGraph( originalFrequencies, tr.evalMany( originalMagnitudes ), Qt::blue );
   gPlotFactory().createScatter( specFrequenciesSorted, tr.evalMany( specMagnitudeSorted ), Plotting::MarkerDrawAttr( Qt::black ) );
   gPlotFactory().createScatter( preselectedFrequencies, tr.evalMany( preselectedMagnitudes ), Plotting::MarkerDrawAttr( Qt::red, Plotting::MarkerCircle ) );
   const RealVector& xEvalBaseline = Utils::createRangeReal( specFrequenciesSorted.front(), specFrequenciesSorted.back(), originalFrequencies.size() * 2 );
   gPlotFactory().createGraph( xEvalBaseline, tr.evalMany( baseline->evalMany( xEvalBaseline ) ), Qt::green );
   gPlotFactory().createScatter( selectedFrequencies, tr.evalMany( selectedMagnitudes ), Plotting::MarkerDrawAttr( Qt::red, Plotting::MarkerCross, 4 ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// createFrequencyProximityPlot
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SrSpecPeakAlgorithm::Monitor::createFrequencyProximityPlot( const std::string& prefix )
{
   size_t nBins = 100;

   Math::RegularAccumArray hist( nBins, 0, 4 );
   for ( size_t i = 0; i < frequencyDistance.size(); ++i )
   {
      hist.add( frequencyDistance[ i ], 1 );
   }

   gPlotFactory().createPlot( prefix + "FrequencyProximity" );
   gPlotFactory().createHistogram( hist );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// createPeakPlot
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SrSpecPeakAlgorithm::Monitor::createPeakPlot( const std::string& prefix )
{
   Math::Log10Function tr;

   gPlotFactory().createPlot( prefix + "PeakPlot" );
   gPlotFactory().createScatter( selectedFrequencies, tr.evalMany( selectedMagnitudes ) );
   gPlotFactory().createScatter( peakFrequencies, tr.evalMany( peakHeights ), Plotting::MarkerDrawAttr( Qt::red, Plotting::MarkerCircle ) );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SrSpecPeakAlgorithm methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SrSpecPeakAlgorithm::SrSpecPeakAlgorithm( double freqProximityCutoff, const std::string& algorithmName, const AlgorithmBase* parent ) :
   AlgorithmBase( algorithmName, parent ),
   m_freqProximityCutoff( freqProximityCutoff )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// execute
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Follow the comments in the code for explanation.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector< Feature::SrSpecPeak > SrSpecPeakAlgorithm::execute( const WaveAnalysis::SrSpectrum& spectrum, Monitor* monitor )
{
   /// The ampCorrectionFactor has been tuned by SrSpecPeakAnalysis (@sse SrSpecPeakAnalysis).
   const double ampCorrectionFactor = 3.95239;

   /// The magic factor of 2.5 has been tuned by SrSpecPeakAnalysis (@see SrSpecPeakAnalysis).
   /// The frequency uncertainty is independent of the frequency, but depends on the window size.
   /// For the magic factor a zero padding size of 0 samples is assumed. It has not been studied how
   /// a larger number zero padding samples influences the frequency uncertainty.
   const double frequencyUncertainty = 2.5 * 1024.0 / spectrum.getConfig().getWindowSize();


   /// Get the reassigned frequencies and magnitudes (magnitude is identical to non-reassigned spectrum).
   const RealVector& frequencies = spectrum.getFrequencies();
   const RealVector& magnitudes = spectrum.getMagnitude();

   /// Sort frequencies.
   SortCache freqSort( frequencies );
   const RealVector& freqSorted = freqSort.applyTo( frequencies );
   const RealVector& magSorted = freqSort.applyTo( magnitudes );

   /// Get the width of a fourier bin. This is used to normalise the frequency proximity cutoff.
   double fourierBinSize = spectrum.getConfig().getFrequencyBinWidth();

   /// peakCandidatePoints will hold indices to points which are candidates for peaks.
   std::vector< size_t > peakCandidatePoints;

   /// bkgPoints is the inverse selection of peakCandidatePoints.
   std::vector< size_t > bkgPoints;

   /// Initialise nearest neighbour distances for the monitor.
   if ( monitor )
   {
      monitor->frequencyDistance = RealVector( freqSorted.size() - 1 );
   }

   /// lnnDist remembers the left nearest neighbour distance in the loop below.
   double lnnDist = std::numeric_limits< double >::max();

   /// Create preselection of peak candidate points if the frequency of the neighbouring point is less than the frequency proximity cutoff.
   for ( size_t i = 0; i < freqSorted.size() - 1; ++i )
   {
      /// Get the normalised distance to the right nearest neighbour.
      double rnnDist = ( freqSorted[ i + 1 ] - freqSorted[ i ] ) / fourierBinSize;
      if ( monitor )
      {
         monitor->frequencyDistance[ i ] = rnnDist;
      }

      if ( rnnDist < m_freqProximityCutoff || lnnDist < m_freqProximityCutoff )
      {
         peakCandidatePoints.push_back( i );
      }
      else
      {
         bkgPoints.push_back( i );
      }

      lnnDist = rnnDist;
   }

   /// Create spectrum values from selection.
   RealVector preselectedFrequencies( peakCandidatePoints.size() );
   RealVector preselectedMagnitudes( peakCandidatePoints.size() );
   for ( size_t i = 0; i < peakCandidatePoints.size(); ++i )
   {
      preselectedFrequencies[ i ] = freqSorted[ peakCandidatePoints[ i ] ];
      preselectedMagnitudes[ i ] = magSorted[ peakCandidatePoints[ i ] ];
   }

   /// Create spectrum values from inverse selection.
   RealVector inverseSelectionFreq( bkgPoints.size() );
   RealVector inverseSelectionMag( bkgPoints.size() );
   for ( size_t i = 0; i < bkgPoints.size(); ++i )
   {
      inverseSelectionFreq[ i ] = freqSorted[ bkgPoints[ i ] ];
      inverseSelectionMag[ i ] = magSorted[ bkgPoints[ i ] ];
   }

   /// Create baseline from all non-peak candidate points.
   Math::SampledMovingAverage movAvgCalc( Math::SampledMovingAverage::createGaussianFilter( 21, 10 ) );
   const RealVector& baselinePoints = movAvgCalc.calculate( inverseSelectionMag );

   RealVector baselineX = inverseSelectionFreq;
   RealVector baselineY = baselinePoints;

   if ( baselineX.size() < 2 )
   {
      getLogger() << Msg::Warning << "Baseline determination failed!" << Msg::EndReq;
      baselineX.clear();
      baselineY.clear();
      baselineX.push_back( freqSorted.front() );
      baselineX.push_back( freqSorted.back() );
      baselineY.push_back( 0 );
      baselineY.push_back( 0 );
   }

   Math::LinearInterpolator baseline( baselineX, baselineY );

   /// Record the points above baseline.
   RealVector magAboveBaseline;
   RealVector freqAboveBaseline;

   for ( size_t i = 0; i < preselectedFrequencies.size(); ++i )
   {
      if ( preselectedMagnitudes[ i ] > baseline( preselectedFrequencies[ i ] ) )
      {
         magAboveBaseline.push_back( preselectedMagnitudes[ i ] );
         freqAboveBaseline.push_back( preselectedFrequencies[ i ] );
      }
   }

   std::vector< Feature::SrSpecPeak > result;
   if ( magAboveBaseline.size() > 0 )
   {

      /// Create peaks from the points above baseline.
      std::vector< std::vector< size_t > > peaks;
      peaks.push_back( std::vector< size_t >() );
      peaks.back().push_back( 0 );

      for ( size_t i = 1; i < freqAboveBaseline.size(); ++i )
      {
         double dist = fabs( freqAboveBaseline[ i ] - freqAboveBaseline[ i - 1 ] ) / fourierBinSize;

         if ( dist < m_freqProximityCutoff * 2 )
         {
            peaks.back().push_back( i );
         }
         else
         {
            peaks.push_back( std::vector< size_t >() );
            peaks.back().push_back( i );
         }
      }


      RealVector peakHeights;
      RealVector peakFreqs;
      for ( size_t i = 0; i < peaks.size(); ++i )
      {
         double maxHeight = -1;
         double maxFreq = 0;
         for ( size_t j = 0; j < peaks[ i ].size(); ++j )
         {
            size_t index = peaks[ i ][ j ];
            if ( magAboveBaseline[ index ] > maxHeight )
            {
               maxHeight = magAboveBaseline[ index ];
               maxFreq = freqAboveBaseline[ index ];
            }
         }
         if ( maxHeight > 0 )
         {
            peakHeights.push_back( maxHeight * ampCorrectionFactor );
            peakFreqs.push_back( maxFreq );
         }
      }

      /// Format peak vector.
      for ( size_t iPeak = 0; iPeak < peakHeights.size(); ++iPeak )
      {
         size_t startTimeSamples = spectrum.getWindowLocation()->getFirstSample();
         size_t endTimeSamples = spectrum.getWindowLocation()->getLastSample();
         result.push_back( Feature::SrSpecPeak( peakFreqs[ iPeak ], peakHeights[ iPeak ], frequencyUncertainty, startTimeSamples, endTimeSamples ) );
      }

      if ( monitor )
      {
         monitor->peakFrequencies = peakFreqs;
         monitor->peakHeights = peakHeights;
      }
   } /// Condition: magAboveBaseline.size() > 0

   /// Set all monitor values except frequencyDistance.
   if ( monitor )
   {
      /// Member frequencyDistance is filled at peak candidate selection.

      monitor->originalFrequencies = spectrum.getConfig().getSpectrumFrequencies();
      monitor->originalMagnitudes = spectrum.getMagnitude();
      monitor->specFrequenciesSorted = freqSorted;
      monitor->specMagnitudeSorted = magSorted;
      monitor->preselectedFrequencies = preselectedFrequencies;
      monitor->preselectedMagnitudes = preselectedMagnitudes;
      monitor->baseline = new Math::LinearInterpolator( baseline );
      monitor->selectedFrequencies = freqAboveBaseline;
      monitor->selectedMagnitudes = magAboveBaseline;
   }

   return result;
}

} /// namespace FeatureAlgorithm
