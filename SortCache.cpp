#include "SortCache.h"

#include <map>
#include <cassert>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SortCache::SortCache( const RealVector& values ) :
   m_translation( values.size() )
{
   std::map< double, int > sortedValueVsIndex;
   for ( size_t i = 0; i < values.size(); ++i )
   {
      sortedValueVsIndex.insert( std::pair< double, int >( values[i], i ) );
   }

   size_t sortedIndex = 0;
   for ( std::map< double, int >::const_iterator it = sortedValueVsIndex.begin(); it != sortedValueVsIndex.end(); ++it, ++sortedIndex )
   {
      m_translation[ sortedIndex ] = it->second;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SortCache::~SortCache()
{}

RealVector SortCache::applyTo( const RealVector& vector ) const
{
   assert( vector.size() == m_translation.size() );
   RealVector result( vector.size() );
   for ( size_t i = 0; i < result.size(); ++i )
   {
      result[i] = vector[ getSortedIndex( i ) ];
   }
   return result;
}

