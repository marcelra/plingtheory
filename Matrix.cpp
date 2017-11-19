#include "Matrix.h"



Matrix::Matrix( size_t nRows, size_t nCols ) :
   m_values( nRows, RealVector( nCols, 0 ) )
{}



Matrix::Matrix( size_t nRows, size_t nCols, double defaultValue ) :
   m_values( nRows, RealVector( nCols, defaultValue ) )
{}



Matrix::Matrix( const std::vector<RealVector>& values ) :
   m_values( values )
{}



RealVector Matrix::getColumn( size_t iCol ) const
{
   assert( iCol < getNumCols() );

   RealVector result(getNumRows());
   for ( size_t iRow = 0; iRow < getNumRows(); ++iRow )
   {
      result[iRow] = m_values[iRow][iCol];
   }

   return result;
}



void Matrix::updateDimensions()
{
   m_numRows = m_values.size();
   m_numCols = m_numRows > 0? m_values.begin()->size() : 0;
}



std::vector<RealVector>& Matrix::getRows()
{
   return m_values;
}
