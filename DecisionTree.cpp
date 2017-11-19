#include "DecisionTree.h"

namespace MlLib
{

DecisionTree::DecisionTree(size_t maxNodes) :
   m_maxDepth(maxNodes)
{}

DecisionTree& DecisionTree::fit(const std::vector<RealVector>& xTrain, const RealVector& yTrain)
{
   return *this;
}

RealVector DecisionTree::predict(const std::vector<RealVector>& x)
{
   return RealVector(x.size(), 0);
}

} /// namespace MlLib
