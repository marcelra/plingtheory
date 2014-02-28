#ifndef REGULARACCUMARRAY_H
#define REGULARACCUMARRAY_H

#include "IAccumArray.h"

class TH1F;

namespace Math
{

/**
 * @class RegularAccumArray
 * @brief Implements an IAccumArray interface. There are a number of bins with equal width and no gaps.
 */
class RegularAccumArray : public IAccumArray
{
   public:
      /**
       * Constructor. Create @param nBins regular bins between @param minX and @param maxX.
       */
      RegularAccumArray( size_t nBins, double minX, double maxX );
      /**
       * Destructor.
       */
      ~RegularAccumArray();

   /**
    * Additional methods for RegularAccumArray.
    */
   public:
      /**
       * Get all the bin contents as RealVector.
       */
      RealVector getAllBinContents() const;
      /**
       * Clear bin contents in @param binIndex, and replace contents with @param value (@see IAccumArray::Bin::set).
       */
      void setBinContent( size_t binIndex, double value );
      /**
       * Initialise all bin contents with @param vec. Asserts that the size of @param vec is equal to the number of bins.
       */
      void setBinContents( const RealVector& vec );

   /**
    * Create a ROOT histogram from the accumulation array.
    */
   public:
      TH1F* createHistogram() const;

   /**
    * IAccumArray interface implementation (@see IAccumArray).
    */
   public:
      size_t getNumBins() const;
      virtual double getMinX() const;
      virtual double getMaxX() const;

      double getBinContent( size_t iBin ) const;
      const TwoTuple& getBinEntries( size_t iBin ) const;

      size_t findBin( double x ) const;
      void add( double x, double value );

      const std::vector< Bin >& getBins() const;
      const Bin& getBin( size_t binIndex ) const;
      Bin& getBin( size_t binIndex );

      double getMissedContent() const;
      double getUnderflow() const;
      double getOverflow() const;
      const Bin& getMissedBin() const;
      const Bin& getUnderflowBin() const;
      const Bin& getOverflowBin() const;

   public:
      std::vector< Bin >      m_bins;
      Bin                     m_underflow;
      Bin                     m_overflow;
      Bin                     m_missed;
      double                  m_binWidth;
};

} /// namespace Math

#endif // REGULARACCUMARRAY_H
