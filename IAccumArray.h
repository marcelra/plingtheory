#ifndef ACCUMARRAY_H
#define ACCUMARRAY_H

#include "RealVector.h"
#include "TwoTuple.h"

namespace Math
{

/**
 * @class IAccumArray
 * @brief Interface class for accumulation arrays.
 * Accumulation should be implemented as a collection of bins that count the weighted occurence of a value between the
 * bounds of the bin. The implementation of IAccumArray::Bin also stores the points that are added.
 *
 * Contents are the integrated, weighted counts.
 * Entries are the values that were added to the bin including weights stored as a TwoTuple.
 *
 * Notes:
 * - Bins should not overlap, other than that, the bins can have any (min, max) as long as max > min.
 * - A point either falls in a regular bin, in an underflow bin, x < getMinX(), an overflow bin, x > getMaxX(). For exotic
 *   bin configurations it is also possible that there are gaps between the bins. In that case the point will be added to the
 *   missed bin.
 */
class IAccumArray
{
   public:
      /**
       * @class Bin
       */
      class Bin
      {
         public:
            /**
             * Construct an empty bin with (minX, maxX).
             */
            Bin( double minX, double maxX );

         public:
            /**
             * Get the left bound.
             */
            double getMinX() const;
            /**
             * Get the right bound.
             */
            double getMaxX() const;
            /**
             * Get bin width
             */
            double getBinWidth() const;

         public:
            /**
             * Get the integrated weighted contents.
             */
            double getContents() const;
            /**
             * Get the bin entries as a TwoTuple.
             */
            const TwoTuple& getEntries() const;
            TwoTuple& getEntries();


            /**
             * Accumulate point @param x, with weight/value @param value. ASsumes that @param x is between min and max.
             * The implementation of add does not check for this.
             */
            void add( double x, double value );
            /**
             * Clear bin entries and set integrated contents to value.
             */
            void set( double value );
            /**
             * Clear bin entries. This also wipes the bin contents.
             */
            void clearEntries();

         private:
            double         m_contents;          //! Weighted counts (contents).
            TwoTuple       m_elements;          //! Entries.
            double         m_minX;              //! Min of bin range.
            double         m_maxX;              //! Max of bin range.
      };

   public:
      /**
       * virtual destructor.
       */
      virtual ~IAccumArray();

   public:
      /**
       * Get the number of bins.
       */
      virtual size_t getNumBins() const = 0;
      /**
       * Get the minimum value of the left edge of all the normal bins (i.e. not including underflow).
       */
      virtual double getMinX() const = 0;
      /**
       * Get the maximum value of the right edge of all the normal bins (i.e. not including overflow).
       */
      virtual double getMaxX() const = 0;

      /**
       * Get the bin contents of bin with index @param iBin. Will not assert that iBin is a correct index.
       */
      virtual double getBinContent( size_t iBin ) const = 0;
      /**
       * Get the bin entries for bin @param iBin. Will not assert that iBin is a correct index.
       */
      virtual const TwoTuple& getBinEntries( size_t iBin ) const = 0;

   public:
      /**
       * Accumulate value x.
       */
      virtual void add( double x, double value ) = 0;

   /**
    * Access bins regular bins.
    */
   public:
      /**
       * Get all the regular bins.
       */
      virtual const std::vector< Bin >& getBins() const = 0;

      /**
       * Find bin corresponding to @param x. @param x should fall within a normal bin (i.e. not including under-, overflow, missed bin).
       * The method should return the same value as getNumBins if @param x does not fall into a regular bin.
       */
      virtual size_t findBin( double x ) const = 0;
      /**
       * Get bin with index @param binIndex.
       */
      virtual const Bin& getBin( size_t binIndex ) const = 0;
      virtual Bin& getBin( size_t binIndex ) = 0;

   /**
    * Access special bins.
    */
   public:
      /**
       * Get the contents of the missed bin.
       */
      virtual double getMissedContent() const = 0;
      /**
       * Get the contents of the underflow bin.
       */
      virtual double getUnderflow() const = 0;
      /**
       * Get the contents of the overflow bin.
       */
      virtual double getOverflow() const = 0;

      /**
       * Get the missed bin.
       */
      virtual const Bin& getMissedBin() const = 0;
      /**
       * Get the underflow bin.
       */
      virtual const Bin& getUnderflowBin() const = 0;
      /**
       * Get the overflow bin.
       */
      virtual const Bin& getOverflowBin() const = 0;

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods IAccumArray::Bin
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double IAccumArray::Bin::getMinX() const
{
   return m_minX;
}

inline double IAccumArray::Bin::getMaxX() const
{
   return m_maxX;
}

inline double IAccumArray::Bin::getBinWidth() const
{
   return m_maxX - m_minX;
}

inline double IAccumArray::Bin::getContents() const
{
   return m_contents;
}

inline void IAccumArray::Bin::add( double x, double value )
{
   assert( x >= m_minX );
   assert( x < m_maxX );
   m_contents += value;
   m_elements.add( x, value );
}

inline const TwoTuple& IAccumArray::Bin::getEntries() const
{
   return m_elements;
}

inline TwoTuple& IAccumArray::Bin::getEntries()
{
   return m_elements;
}

inline void IAccumArray::Bin::set( double value )
{
   clearEntries();
   add( 0.5 * ( getMinX() + getMaxX() ), value );
}

inline void IAccumArray::Bin::clearEntries()
{
   m_elements = TwoTuple();
   m_contents = 0;
}

} /// namespace Math

#endif // ACCUMARRAY_H
