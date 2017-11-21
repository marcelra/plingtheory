#ifndef MLLIBUTILS_H
#define MLLIBUTILS_H

#include "Matrix.h"

namespace MlLib
{

template< class T, class U >
class TrainTestSplit
{
   public:
      /**
       * Create a train-test split based on @param ratio. @param ratio should be between 0 and 1.
       */
      TrainTestSplit( const std::vector<T>& x, const std::vector<U>& y, double ratio );

      std::vector<T> xTrain;
      std::vector<T> xTest;
      std::vector<U> yTrain;
      std::vector<U> yTest;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TrainTestSplit template methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template< class T, class U >
TrainTestSplit<T, U>::TrainTestSplit( const std::vector<T>& x, const std::vector<U>& y, double ratio )
{
   assert( ratio < 1 );
   assert( x.size() == y.size() );

   size_t nTrain = x.size() * ratio;

   xTrain = std::vector<T>( x.begin(), x.begin() + nTrain );
   xTest = std::vector<T>( x.begin() + nTrain + 1, x.end() );

   yTrain = std::vector<U>( y.begin(), y.begin() + nTrain );
   yTest = std::vector<U>( y.begin() + nTrain + 1, y.end() );
}

} /// namespace MlLib

#endif // MLLIBUTILS_H
