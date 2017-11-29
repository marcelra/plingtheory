#ifndef DECISIONTREENODES_H
#define DECISIONTREENODES_H

#include "Matrix.h"
#include "RealVector.h"


namespace MlLib {

typedef std::vector<size_t> IndexVector;

class TempArrays
{
   public:
      TempArrays( size_t size ) :
         sumSquaresLeft(size),
         sumSquaresRight(size),
         sumTermLeft(size),
         sumTermRight(size)
      {}

   public:
      RealVector sumSquaresLeft;
      RealVector sumSquaresRight;

      RealVector sumTermLeft;
      RealVector sumTermRight;
};


class IDecisionTreeGraphElement
{
   public:
      virtual ~IDecisionTreeGraphElement() {}
      virtual double eval( const RealVector& x ) const = 0;
};



class AbsDecisionTreeNode : public MlLib::IDecisionTreeGraphElement
{
   public:
      AbsDecisionTreeNode();
      virtual ~AbsDecisionTreeNode();

      double eval( const RealVector& x ) const;

   protected:
      IDecisionTreeGraphElement*     m_leftChildNode;
      IDecisionTreeGraphElement*     m_rightChildNode;
      size_t                         m_splitFeatureIndex;
      double                         m_splitValue;
};



class DecisionTreeSplit
{
   public:
      DecisionTreeSplit( const Matrix& xTrain, const RealVector& yTrain, size_t splitFeature, double splitValue );

   public:
      Matrix       xLeft;
      Matrix       xRight;
      RealVector   yLeft;
      RealVector   yRight;
};



class DecisionTreeNode : public AbsDecisionTreeNode
{
   public:
      DecisionTreeNode( size_t depth, size_t maxDepth, const Matrix& xTrain, const RealVector& yTrain );

   private:
      DecisionTreeSplit findBestSplit( const Matrix& xTrain, const RealVector& yTrain );
};



class DecisionTreeLeafNode : public MlLib::IDecisionTreeGraphElement
{
   public:
      DecisionTreeLeafNode( double value );
      double eval( const RealVector& x ) const;

   private:
      double      m_value;
};



class DecisionTreeSplitRef
{
   public:
      DecisionTreeSplitRef( const Matrix& xTrain, const RealVector& yTrain, size_t splitFeature, double splitValue, const IndexVector& indexVector );

      IndexVector indexLeft;
      IndexVector indexRight;
};



class DecisionTreeRefNode : public AbsDecisionTreeNode
{
   public:
      DecisionTreeRefNode( size_t depth, size_t maxDepth, const Matrix& xTrain, const RealVector& yTrain, const IndexVector& indices, TempArrays& tempArrays );

   private:
      DecisionTreeSplitRef findBestSplit( const Matrix& xTrain, const RealVector& yTrain, const IndexVector& indices, TempArrays& tempArrays );
};




} /// namespace MlLib

#endif // DECISIONTREENODES_H
