#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include "RealVector.h"

namespace MlLib
{



class IDecisionTreeGraphElement
{
   public:
      virtual ~IDecisionTreeGraphElement() {}
      virtual double eval( const RealVector& x ) const = 0;
};



class DecisionTree
{
   public:
      DecisionTree( size_t maxDepth );

      DecisionTree& fit( const std::vector<RealVector>& xTrain, const RealVector& yTrain );
      RealVector predict( const std::vector<RealVector>& x );

   private:
      size_t m_maxDepth;

      IDecisionTreeGraphElement*   m_tree;

};

} /// namespace MlLib

#endif // DECISIONTREE_H
