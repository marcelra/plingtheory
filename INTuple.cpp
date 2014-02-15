#include "INTuple.h"

#include "RealVector.h"

#include <cassert>
#include <map>

namespace Math
{

INTuple::~INTuple()
{}

INTuple::Ptr INTuple::sortVariable( size_t variableIndex ) const
{
   INTuple::Ptr result = this->clone();

   const RealVector& vec = getVariable( variableIndex );
   std::map< double, int > sortedVarVsRowIndex;
   for ( size_t i = 0; i < vec.size(); ++i )
   {
      sortedVarVsRowIndex[ vec[i] ] = i;
   }

   size_t sortedIndex = 0;
   for ( std::map< double, int >::const_iterator it = sortedVarVsRowIndex.begin(); it != sortedVarVsRowIndex.end(); ++it, ++sortedIndex )
   {
      result->setRow( sortedIndex, getRow( it->second ) );
   }

   return result;
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
