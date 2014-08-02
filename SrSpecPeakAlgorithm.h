#ifndef SRSPECPEAKALGORITHM_H
#define SRSPECPEAKALGORITHM_H

#include "AlgorithmBase.h"
#include "RealVector.h"

#include <vector>

/// Forward declarations.
namespace WaveAnalysis
{
class SrSpectrum;
}

namespace Math
{
class LinearInterpolator;
class IRealFunction;
}

namespace Feature
{

/**
 * @class SrSpecPeak
 * @brief Simple reassigned Fourier spectrum peak feature that holds the peak frequency and its height.
 */
class SrSpecPeak
{
   public:
      /**
       * Create a SrSpecPeak with frequency @param frequency and height @param height.
       */
      SrSpecPeak( double frequency, double height );

      /**
       * Get the frequency of the peak.
       */
      double getFrequency() const;

      /**
       * Get the height of the peak.
       */
      double getHeight() const;

   private:
      double      m_frequency;      //! Frequency of the peak.
      double      m_height;         //! Height of the peak.
};

} /// namespace Feature



namespace FeatureAlgorithm
{

/**
 * @class SrSpecPeakAlgorithm
 * @brief Algorithm for finding peaks in reassigned spectra.
 * For a description of the algorithm @see SrSpecPeakAlgorithm::execute.
 */
class SrSpecPeakAlgorithm : public AlgorithmBase
{
   public:
      /**
       * @class Monitor
       * @brief A pointer of this class can be passed to SrSpecPeakAlgorithm::execute. After the execute it is possible
       * to create plots and query algorithm internal data. All data is copied in this class and can be queried publicly.
       */
      class Monitor
      {
         public:
            /**
             * Constructor.
             */
            Monitor();
            /**
             * Destructor.
             */
            ~Monitor();

            /**
             * Create a spectrum plot showing:
             *  - The original Fourier spectrum in blue.
             *  - The reassigned spectrum with black plus signs.
             *  - Preselected peak points in red circles.
             *  - Baseline in green.
             *  - Selected peak points in large red crosses.
             */
            void createSpectrumPlot( const std::string& prefix );

            /**
             * Create a histogram of the nearest neighbour proximity. If the nearest neighbour proximity is below the
             * frequency proximity cutoff, the points are associated to form a preselected peak candidate (@see SrSpecPeakAlgorithm).
             */
            void createFrequencyProximityPlot( const std::string& prefix );

            /**
             * Create a plot showing the peaks:
             *  - The points forming the peak are displayed in black.
             *  - The highest point (i.e. the actual peak returned by the algorithm) in red circles.
             */
            void createPeakPlot( const std::string& prefix );

         /**
          * Blocked assignment operator and copy constructor (@see member baseline).
          */
         private:
            Monitor( const Monitor& other );
            Monitor& operator=( const Monitor& other );

         public:
            RealVector                originalFrequencies;        //! The original frequencies from the Fourier spectrum.
            RealVector                originalMagnitudes;         //! The original magnitudes from the Fourier spectrum.
            RealVector                specFrequenciesSorted;      //! The sorted frequencies from the reassigned spectrum.
            RealVector                specMagnitudeSorted;        //! The magnitudes sorted along frequency from the reassigned spectrum.
            RealVector                frequencyDistance;          //! The nearest neighbour distance from the sorted frequencies.
            RealVector                preselectedFrequencies;     //! The preselected frequencies.
            RealVector                preselectedMagnitudes;      //! The preselected magnitudes.
            Math::LinearInterpolator* baseline;                   //! The baseline.
            RealVector                selectedFrequencies;        //! The selected frequencies (preselection above baseline).
            RealVector                selectedMagnitudes;         //! The selected magnitudes.
            RealVector                peakFrequencies;            //! The return peak frequencies.
            RealVector                peakHeights;                //! The return peak heights.
      };

   public:
      /**
       * Create an SrSpecPeakAlgorithm with @param freqProximityCutoff. When two neighbouring points in the reassigned
       * spectrum have difference in frequency that is less than this value, they are preselected as peak candidate.
       * The frequency proximity cutoff is normalised to fourier bin size.
       * For other parameters @see AlgorithmBase.
       */
      SrSpecPeakAlgorithm( double freqProximityCutoff = 0.25, const std::string& name = "SrSpecPeakAlgorithm", const AlgorithmBase* parent = 0 );
      std::vector< Feature::SrSpecPeak > execute( const WaveAnalysis::SrSpectrum& spectrum, Monitor* monitor = 0 );

   private:
      double         m_freqProximityCutoff;        //! Frequency proximity cut off.
};

} /// namespace FeatureAlgorithm



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods SrSpecPeak
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Feature
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getFrequency
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double SrSpecPeak::getFrequency() const
{
   return m_frequency;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getHeight
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double SrSpecPeak::getHeight() const
{
   return m_height;
}

} /// namespace Feature

#endif // SRSPECPEAKALGORITHM_H
