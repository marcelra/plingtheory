#ifndef RAWPCMDATA_H
#define RAWPCMDATA_H

#include "Exceptions.h"
#include "RealVector.h"
#include "SamplingInfo.h"

#include <vector>
#include <memory>

/**
 * @class RawPcmData
 * @brief Represents PCM data in double format between [0, 1]
 *
 * Contains a SamplingInfo object.
 * Has methods for mixing, cutting and pasting.
 */
class RawPcmData : private std::vector<double>
{
   public:
      /**
       * Construct empty object
       * @param sampleInfo @see SamplingInfo
       */
      explicit RawPcmData( const SamplingInfo& sampleInfo );
      /**
       * Create uninitialised object of length @param numSamples
       */
      explicit RawPcmData( const SamplingInfo& samplingInfo, size_t numSamples );
      /**
       * Create object of length @param numSamples, initialise with @param initialVal
       */
      explicit RawPcmData( const SamplingInfo& samplingInfo, size_t numSamples, double initialVal );
      /**
       * Initialise RawPcmData from double array iterators @param first and @param last
       */
      explicit RawPcmData( const SamplingInfo& samplingInfo, const double* first, const double* last );

      /**
       * Access the samplingInfo object
       */
      const SamplingInfo& getSamplingInfo() const;
      SamplingInfo& getSamplingInfo();

      /**
       * Converts (normalises) @param value and assigns it as sample index @param iSample.
       * Usually normalisation yields values between [-1, 1], but note that this method does not guarantee that.
       */
      void setUnnormalisedSample( size_t iSample, double value );
      /**
       * Maps the normalised data to max value possible from the samplingInfo
       */
      double getUnnormalisedSample( size_t iSample ) const;

      /**
       * Dump all the sample values to @param os
       */
      void dump( std::ostream& os ) const;
      /**
       * Pastes @param rawPcmData at the end of this instance
       */
      void pasteAtEnd( const RawPcmData& rawPcmData );
      /**
       * Mix @param other with this pcm data. @param other is assumed to be smaller or equally sized to the instance on which this
       * method is called. The offset @param offset indicates from which sample in the original pcm data the mixing should begin.
       * Note offset + other.size() is required to be smaller than this->size()
       */
      void mixAdd( const RawPcmData& other, size_t offset = 0 );
      /**
       * Renormalise the current pcm data to the highest value found
       */
      void normaliseToPeak();

      /**
       * copies the contents of this to (true, note private inheritance) vector<double> format
       */
      RealVectorPtr copyToVectorData() const;

      typedef std::vector<double> vecD;
      typedef std::unique_ptr<RawPcmData> Ptr;

   /**
    * Expose parts of the vector interface
    */
   public:
      using vecD::operator[];
      using vecD::at;
      using vecD::size;
      using vecD::push_back;
      using vecD::front;
      using vecD::back;
      using vecD::insert;
      using vecD::resize;
      using vecD::begin;
      using vecD::end;
      using vecD::rbegin;
      using vecD::rend;

   private:
      SamplingInfo  m_samplingInfo;
};

////////////////////////////////////////////////////////////////////////////////
/// Inline methods RawPcmData
////////////////////////////////////////////////////////////////////////////////
inline const SamplingInfo& RawPcmData::getSamplingInfo() const
{
   return const_cast< RawPcmData* >(this)->getSamplingInfo();
}

inline SamplingInfo& RawPcmData::getSamplingInfo()
{
   return m_samplingInfo;
}

inline void RawPcmData::setUnnormalisedSample( size_t iSample, double value )
{
   (*this)[iSample] = value * getSamplingInfo().getNormalisationFactor();
}

inline double RawPcmData::getUnnormalisedSample( size_t iSample ) const
{
   return ( *this )[iSample] * getSamplingInfo().getMaxValue();
}

#endif // RAWPCMDATA_H
