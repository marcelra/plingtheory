#ifndef REGULARACCUMARRAY_H
#define REGULARACCUMARRAY_H

#include "IAccumArray.h"

namespace Math
{

class RegularAccumArray : public IAccumArray
{
   public:
      RegularAccumArray( size_t nBins, double minX, double maxX );
      ~RegularAccumArray();

   public:
      double getBinContent( size_t iBin ) const;
      double getMissedContent() const;
      double getUnderflow() const;
      double getOverflow() const;

      const TwoTuple& getBinEntries( size_t iBin ) const;
      const std::vector< Bin >& getBins() const;

      size_t getNumBins() const;
      virtual double getMinX() const;
      virtual double getMaxX() const;

      size_t findBin( double x ) const;
      Bin getBin( size_t binIndex ) const;
      void add( double x, double value );

   public:
      std::vector< Bin >      m_bins;
      Bin                     m_underflow;
      Bin                     m_overflow;
      double                  m_binWidth;

};

} /// namespace Math

#endif // REGULARACCUMARRAY_H
