#ifndef MATRIX_H
#define MATRIX_H

#include "RealVector.h"



class Matrix
{
   public:
      Matrix(size_t nRows, size_t nCols);
      Matrix(size_t nRows, size_t nCols, double defaultValue);
      Matrix(const std::vector<RealVector>& values);

      RealVector getColumn(size_t col) const;

      double& at(size_t iRow, size_t iCol);
      std::vector<RealVector>& getRows();

      size_t getNumRows() const;
      size_t getNumCols() const;

   private:
      void updateDimensions();

   private:
      std::vector<RealVector>    m_values;
      size_t                     m_numRows;
      size_t                     m_numCols;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline size_t Matrix::getNumRows() const
{
   return m_values.size();
}



inline size_t Matrix::getNumCols() const
{
   return m_values[0].size();
}



inline double& Matrix::at(size_t iRow, size_t iCol)
{
   return m_values[iRow][iCol];
}


#endif // MATRIX_H
