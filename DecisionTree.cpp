#include "DecisionTree.h"

#include "SortCache.h"
#include "Utils.h"
#include "Matrix.h"
#include "Logger.h"

#include <iostream>

namespace
{

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

class DecisionTreeNode : public MlLib::IDecisionTreeGraphElement
{
   public:
      DecisionTreeNode( size_t depth, size_t maxDepth, const Matrix& xTrain, const RealVector& yTrain );
      virtual ~DecisionTreeNode();

      double eval( const RealVector& x ) const;

   private:
      DecisionTreeSplit findBestSplit( const Matrix& xTrain, const RealVector& yTrain );

   private:
      IDecisionTreeGraphElement*     m_leftChildNode;
      IDecisionTreeGraphElement*     m_rightChildNode;
      size_t                         m_splitFeatureIndex;
      double                         m_splitValue;
};

class DecisionTreeLeafNode : public MlLib::IDecisionTreeGraphElement
{
   public:
      DecisionTreeLeafNode( const RealVector& yTrain );
      double eval( const RealVector& x ) const;

   private:
      double      m_value;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// DecisionTreeSplit implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DecisionTreeSplit::DecisionTreeSplit( const Matrix& xTrain, const RealVector& yTrain, size_t splitFeature, double splitValue )
{
   assert ( xTrain.getNumRows() == yTrain.size() );

   std::vector< size_t > leftIndices;
   leftIndices.reserve( xTrain.getNumRows() );
   std::vector< size_t > rightIndices;
   rightIndices.reserve( xTrain.getNumRows() );
   for ( size_t iSample = 0; iSample < xTrain.getNumRows(); ++iSample )
   {
      if ( xTrain.at( iSample, splitFeature ) < splitValue )
      {
         leftIndices.push_back( iSample );
      }
      else
      {
         rightIndices.push_back( iSample );
      }
   }

   xLeft = Matrix( applyIndexing( xTrain.getRows(), leftIndices ) );
   yLeft = applyIndexing( yTrain, leftIndices );
   xRight = Matrix( applyIndexing( xTrain.getRows(), rightIndices ) );
   yRight = applyIndexing( yTrain, rightIndices );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// DecisionTreeNode implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DecisionTreeNode::DecisionTreeNode( size_t depth, size_t maxDepth, const Matrix& xTrain, const RealVector& yTrain ) :
   m_leftChildNode(0),
   m_rightChildNode(0),
   m_splitFeatureIndex(0),
   m_splitValue(0)
{
   if ( yTrain.size() >= 2 )
   {
      DecisionTreeSplit&& split = findBestSplit( xTrain, yTrain );

      // std::cout << "Todo: " << todo << ": Depth: " << depth << ", split @ " << m_splitValue << ", left = " << split.yLeft.size() << " right = " << split.yRight.size() << std::endl;

      if ( depth < maxDepth )
      {
         m_leftChildNode = new DecisionTreeNode( depth + 1, maxDepth, split.xLeft, split.yLeft );
         m_rightChildNode = new DecisionTreeNode( depth + 1, maxDepth, split.xRight, split.yRight );
      }
      else
      {
         m_leftChildNode = new DecisionTreeLeafNode( split.yLeft );
         m_rightChildNode = new DecisionTreeLeafNode( split.yRight );
      }
   }
   else
   {
      /// TODO: should be able to replace by assert?!
      double leafVal = yTrain.size() > 0? yTrain[0] : 0;
      m_leftChildNode = new DecisionTreeLeafNode( realVector( leafVal ) );
      m_rightChildNode = new DecisionTreeLeafNode( realVector( leafVal ) );
   }
}

DecisionTreeNode::~DecisionTreeNode()
{
   delete m_leftChildNode;
   delete m_rightChildNode;
}

double DecisionTreeNode::eval( const RealVector& x ) const
{
   if ( x[ m_splitFeatureIndex ] <= m_splitValue )
   {
      return m_leftChildNode->eval( x );
   }
   else
   {
      return m_rightChildNode->eval( x );
   }
}

DecisionTreeSplit DecisionTreeNode::findBestSplit(const Matrix& xTrain, const RealVector& yTrain )
{
   const size_t nPoints = yTrain.size();
   assert ( xTrain.getNumRows() == yTrain.size() );
   assert( yTrain.size() > 0 );
   const size_t nFeats = xTrain.getNumCols();

   std::vector< double > improvements( nFeats );
   std::vector< double > candidateSplits( nFeats );

   if ( nPoints == 2 )
   {
      const RealVector& delta = xTrain.getRow(0) - xTrain.getRow(1);
      size_t featureIndex;
      Utils::getMaxValueAndIndex( delta, featureIndex );
      m_splitValue = (xTrain.at( 0, featureIndex ) + xTrain.at( 1, featureIndex ) ) / 2;
      m_splitFeatureIndex = featureIndex;
      return DecisionTreeSplit( xTrain, yTrain, m_splitFeatureIndex, m_splitValue );
   }

   for ( size_t iFeat = 0; iFeat < nFeats; ++iFeat )
   {
      const RealVector& featureData = xTrain.getColumn( iFeat );
      SortCache sc( featureData );
      const RealVector& sortedFeatureData = sc.applyTo( featureData );
      RealVector&& ySorted = sc.applyTo(yTrain);

      RealVector sumSquaresLeft(ySorted.size());
      RealVector sumSquaresRight(ySorted.size());
      RealVector sumTermLeft(ySorted.size());
      RealVector sumTermRight(ySorted.size());

      sumSquaresLeft[0] = ySorted[0] * ySorted[0];
      sumTermLeft[0] = ySorted[0];
      for ( size_t iLeftIt = 1; iLeftIt < ySorted.size(); ++iLeftIt )
      {
         sumSquaresLeft[iLeftIt] = sumSquaresLeft[iLeftIt - 1] + ySorted[iLeftIt] * ySorted[iLeftIt];
         sumTermLeft[iLeftIt] = sumTermLeft[iLeftIt - 1] + ySorted[iLeftIt];
      }

      /// Todo can't we use some smarter rule here total sum squares = sum right + sum left?
      size_t lastIdx = ySorted.size() - 1;
      sumSquaresRight[lastIdx] = ySorted[lastIdx] * ySorted[lastIdx];
      sumTermRight[lastIdx] = ySorted[lastIdx];
      for ( int iRightIt = lastIdx - 1; iRightIt >= 0; --iRightIt )
      {
         sumSquaresRight[iRightIt] = sumSquaresRight[iRightIt + 1] + ySorted[iRightIt] * ySorted[iRightIt];
         sumTermRight[iRightIt] = sumTermRight[iRightIt + 1] + ySorted[iRightIt];
      }

      // RealVector meanTermLeft( sumSquaresLeft.size() );
      // RealVector meanTermRight( sumSquaresRight.size() );

      for ( size_t i = 0; i < sumTermLeft.size(); ++i )
      {
         sumTermLeft[i] = sumTermLeft[i] * sumTermLeft[i] / ( i + 1 );
         sumTermRight[lastIdx - i] = sumTermRight[lastIdx - i] * sumTermRight[lastIdx - i] / ( i + 1 );
      }

      // const RealVector& stdDevLeft = sumSquaresLeft - sumTermLeft;
      // const RealVector& stdDevRight = sumSquaresRight - sumTermRight;

      // const RealVector& sumStdDev = stdDevLeft + stdDevRight;

      RealVector sumStdDev( ySorted.size() );
      for ( size_t i = 0; i < ySorted.size(); ++i )
      {
         sumStdDev[i] = sumSquaresLeft[i] + sumSquaresRight[i] - sumTermLeft[i] - sumTermRight[i];
      }

      size_t splitIndex = 0;
      double minValue = Utils::getMinValueAndIndex( sumStdDev, splitIndex );
      double improvement = sumStdDev[0] - minValue;

      improvements[iFeat] = improvement;
      candidateSplits[iFeat] = sortedFeatureData[splitIndex];
      if ( splitIndex < sortedFeatureData.size() )
      {
         candidateSplits[iFeat] += sortedFeatureData[splitIndex + 1];
         candidateSplits[iFeat] /= 2;
      }
   }

   size_t iBestSplit;
   Utils::getMaxValueAndIndex( improvements, iBestSplit );

   m_splitValue = candidateSplits[iBestSplit];
   m_splitFeatureIndex = iBestSplit;

   return DecisionTreeSplit( xTrain, yTrain, m_splitFeatureIndex, m_splitValue );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// DecisionTreeLeafNode implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DecisionTreeLeafNode::DecisionTreeLeafNode( const RealVector& yTrain ) :
   m_value( calcMean(yTrain) )
{}

double DecisionTreeLeafNode::eval( const RealVector& x ) const
{
   return m_value;
}


} /// anynomous namespace

namespace MlLib
{

DecisionTree::DecisionTree( size_t maxDepth ) :
   m_maxDepth( maxDepth ),
   m_tree( 0 )
{}

DecisionTree& DecisionTree::fit( const std::vector<RealVector>& xTrain, const RealVector& yTrain )
{
   Logger msg( "DecisionTree" );

   msg << Msg::Info << "Fitting decision tree..." << Msg::EndReq;

   /// TODO: casting it to matrix. Consider changing signature.
   const Matrix& xAsMatrix( xTrain );

   m_tree = new DecisionTreeNode( 0, m_maxDepth, xAsMatrix, yTrain );

   return *this;
}

RealVector DecisionTree::predict( const std::vector<RealVector>& x )
{
   assert ( m_tree != 0 );
   RealVector result( x.size() );

   for ( size_t iSample = 0; iSample < x.size(); ++iSample )
   {
      result[iSample] = m_tree->eval( x[iSample] );
   }
   return result;
}

} /// namespace MlLib
