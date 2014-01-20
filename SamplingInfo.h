#ifndef SAMPLINGINFO_H
#define SAMPLINGINFO_H

/**
 * @class SamplingInfo
 * @brief Contains sampling information for PCM objects
 *
 * Container for information about the sampling rate, and sampling resolution.
 * This class also provides some calculations that are determined by the sampling. Those methods are documented below.
 */
class SamplingInfo
{
   public:
      /**
       * Empty constructor
       * Constructs sampling info with max sample value and samplingRate set to default
       */
      SamplingInfo();
      /**
       *
       */
      explicit SamplingInfo( double samplingRate, double maxValue = 32767 );

      /**
       * Set the sampling rate to @param samplingRate
       */
      void   setSamplingRate( double samplingRate );
      /**
       * Access the sampling rate
       */
      double getSamplingRate() const;
      /**
       * Get the Nyquist frequency
       */
      double getNyquistFrequency() const;

      /**
       * Set the max sample value
       */
      void setMaxValue( double maxValue );
      /**
       * Get the max sample value
       */
      double getMaxValue() const;
      /**
       * Get normalisation factor (i.e. 1/maxValue)
       */
      double getNormalisationFactor() const;

      /**
       * Comparison operators
       */
      bool operator==( const SamplingInfo& other ) const;
      bool operator!=( const SamplingInfo& other ) const;

   public:
      /**
       * Calculate the phase step per sample for frequency @param frequency in radians
       */
      double getPhaseStepPerSample( double frequency ) const;
      /**
       * Get the period of @param frequency in samples
       */
      double getPeriodInSamples( double frequency ) const;
      /**
       * Calclate the number of samples of a period given in seconds
       */
      double convertSecsToSamples( double periodInSecs ) const;

   private:
      double      m_samplingRate;
      double      m_maxValue;
      double      m_normalisationFactor;
};

////////////////////////////////////////////////////////////////////////////////
/// Inline methods SamplingInfo
////////////////////////////////////////////////////////////////////////////////
inline void SamplingInfo::setSamplingRate( double samplingRate )
{
   this->m_samplingRate = samplingRate;
}

inline double SamplingInfo::getSamplingRate() const
{
   return m_samplingRate;
}

inline double SamplingInfo::getNyquistFrequency() const
{
   return m_samplingRate / 2.0;
}

inline bool SamplingInfo::operator==( const SamplingInfo& other ) const
{
   return ! ( *this != other );
}

inline bool SamplingInfo::operator!=( const SamplingInfo& other ) const
{
   if ( m_samplingRate != other.m_samplingRate )
      return true;
   return false;
}

inline void SamplingInfo::setMaxValue( double maxValue )
{
   m_maxValue = maxValue;
   m_normalisationFactor = 1.0/maxValue;
}

inline double SamplingInfo::getMaxValue() const
{
   return m_maxValue;
}

inline double SamplingInfo::getNormalisationFactor() const
{
   return m_normalisationFactor;
}

#endif // SAMPLINGINFO_H
