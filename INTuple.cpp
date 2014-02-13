#include "INTuple.h"

#include "RealVector.h"

#include <cassert>
#include <map>

namespace Math
{

INTuple::~INTuple()
{}

void INTuple::sortVariable( size_t variableIndex )
{
   const RealVector& vec = getVariable( variableIndex );

   std::map< double, int > sortedVarVsRowIndex;
   for ( size_t i = 0; i < vec.size(); ++i )
   {
      sortedVarVsRowIndex[ vec[i] ] = i;
   }

   std::map< int, RealVector > storedRows;
   size_t sortedIndex = 0;
   for ( std::map< double, int >::const_iterator it = sortedVarVsRowIndex.begin(); it != sortedVarVsRowIndex.end(); ++it, ++sortedIndex )
   {
      size_t swapIndex = it->second;
      if ( sortedIndex == swapIndex )
      {
         continue;
      }

      std::map< int, RealVector >::iterator itData = storedRows.find( swapIndex );
      if ( itData == storedRows.end() )
      {
         storedRows[ sortedIndex ] = getRow( sortedIndex );
         setRow( sortedIndex, getRow( swapIndex ) );
      }
      else
      {
         storedRows[ sortedIndex ] = getRow( sortedIndex );
         setRow( sortedIndex, itData->second );
         storedRows.erase( itData );
      }
   }
}

RealVector INTuple::getRow( size_t rowIndex ) const
{
   RealVector result( getNumVariables() );
   for ( size_t iVar = 0; iVar < getNumVariables(); ++iVar )
   {
      result[iVar] = getVariable( iVar )[ rowIndex ];
   }
   return result;
}

void INTuple::setRow( size_t rowIndex, const RealVector& row )
{
   assert( row.size() == getNumVariables() );
   for ( size_t iVar = 0; iVar < row.size(); ++iVar )
   {
      getVariable( iVar )[ rowIndex ] = row[ iVar ];
   }
}

} /// namespace Math
