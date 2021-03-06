#ifndef SORTCACHE_H
#define SORTCACHE_H

#include "RealVector.h"

/**
 * @class SortCache
 * @brief Class that provides indices to sorted variables. Helper class to loop over unsorted vectors such that they
 * appear to be sorted. The index of the loop can be translated using the getSortedIndex method.
 */
class SortCache
{
   public:
      /**
       * Create a SortCache based on @param Values.
       */
      SortCache( const RealVector& values );
      /**
       * Destructor.
       */
      virtual ~SortCache();

      /**
       * Get the index in the original vector of the @index-th item sorted by value.
       */
      size_t getSortedIndex( size_t index ) const;
      /**
       * Get the index in the original vector of the @index-th item reverse sorted by value (i.e. large items first).
       */
      size_t getReverseSortedIndex( size_t index ) const;
      /**
       * Get the number of values after sort (this can be different due to duplicates).
       */
      size_t getSize() const;

      /**
       * Sort vector.
       */
      RealVector applyTo( const RealVector& vector ) const;
      /**
       * Sort vector reverse (i.e. from large to small).
       */
      RealVector applyReverseTo( const RealVector& vector ) const;

   private:
      std::vector< size_t >   m_sortedIndices;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline size_t SortCache::getSortedIndex( size_t index ) const
{
   return m_sortedIndices[ index ];
}

inline size_t SortCache::getReverseSortedIndex( size_t index ) const
{
   return *(m_sortedIndices.rbegin() + index);
}

#endif // SORTCACHE_H
