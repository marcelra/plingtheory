#include "DecisionTreeNodes.h"

#include "Utils.h"
#include "SortCache.h"

namespace MlLib
{

} /// namespace MlLib


namespace
{

RealVector computeSumStdLeftRight( const RealVector& ySorted, MlLib::TempArrays& arr )
{
   RealVector& sumSquaresLeft = arr.sumSquaresLeft;
   RealVector& sumSquaresRight = arr.sumSquaresRight;
   RealVector& sumTermLeft = arr.sumTermLeft;
   RealVector& sumTermRight = arr.sumTermRight;
   // RealVector sumSquaresRight(ySorted.size());
   // RealVector sumTermLeft(ySorted.size());
   // RealVector sumTermRight(ySorted.size());

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

   for ( size_t i = 0; i < ySorted.size(); ++i )
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

   return sumStdDev;
}

} /// anonymous namespace


namespace MlLib {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AbsDecisionTreeNode
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AbsDecisionTreeNode::AbsDecisionTreeNode() :
   m_leftChildNode(0),
   m_rightChildNode(0),
   m_splitFeatureIndex(0),
   m_splitValue(0)
{}



AbsDecisionTreeNode::~AbsDecisionTreeNode()
{
   delete m_leftChildNode;
   delete m_rightChildNode;
}



double AbsDecisionTreeNode::eval( const RealVector& x ) const
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



DecisionTreeSplitRef::DecisionTreeSplitRef( const Matrix& xTrain, const RealVector& yTrain, size_t splitFeature, double splitValue, const IndexVector& indexVector )
{
   assert( indexVector.size() > 1 );

   indexLeft.reserve(indexVector.size());
   indexRight.reserve(indexVector.size());

   for ( auto it = indexVector.begin(); it != indexVector.end(); ++it )
   {
      const size_t index = *it;
      if ( xTrain.at( index, splitFeature ) < splitValue )
      {
         indexLeft.push_back(index);
      }
      else
      {
         indexRight.push_back(index);
      }
   }

   if ( indexLeft.size() == 0 )
   {
      indexLeft.push_back(indexRight[0]);
      indexRight.erase(indexRight.begin());
   }
   if ( indexRight.size() == 0 )
   {
      indexRight.push_back(indexLeft[0]);
      indexLeft.erase(indexLeft.begin());
   }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// DecisionTreeNode implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DecisionTreeNode::DecisionTreeNode( size_t depth, size_t maxDepth, const Matrix& xTrain, const RealVector& yTrain ) :
   AbsDecisionTreeNode()
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
         m_leftChildNode = new DecisionTreeLeafNode( calcMean( split.yLeft ) );
         m_rightChildNode = new DecisionTreeLeafNode( calcMean( split.yRight ) );
      }
   }
   else
   {
      /// TODO: should be able to replace by assert?!
      double leafVal = yTrain.size() > 0? yTrain[0] : 0;
      m_leftChildNode = new DecisionTreeLeafNode( leafVal );
      m_rightChildNode = new DecisionTreeLeafNode( leafVal );
   }
}



DecisionTreeSplit DecisionTreeNode::findBestSplit( const Matrix& xTrain, const RealVector& yTrain )
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
DecisionTreeLeafNode::DecisionTreeLeafNode( double value ) :
   m_value( value )
{}

double DecisionTreeLeafNode::eval( const RealVector& x ) const
{
   return m_value;
}



DecisionTreeRefNode::DecisionTreeRefNode( size_t depth, size_t maxDepth, const Matrix& xTrain, const RealVector& yTrain, const IndexVector& indices, TempArrays& tempArrays ) :
   AbsDecisionTreeNode()
{
   if ( indices.size() >= 2 )
   {
      DecisionTreeSplitRef split = findBestSplit( xTrain, yTrain, indices, tempArrays );

      if ( depth < maxDepth )
      {
         if ( split.indexLeft.size() > 1 )
         {
            m_leftChildNode = new DecisionTreeRefNode( depth + 1, maxDepth, xTrain, yTrain, split.indexLeft, tempArrays );
         }
         else if ( split.indexLeft.size() == 1 )
         {
            m_leftChildNode = new DecisionTreeLeafNode( yTrain[split.indexLeft[0]] );
         }
         else
         {
            assert( false );
         }

         if ( split.indexRight.size() > 1 )
         {
            m_rightChildNode = new DecisionTreeRefNode( depth + 1, maxDepth, xTrain, yTrain, split.indexRight, tempArrays );
         }
         else if ( split.indexRight.size() == 1 )
         {
            m_rightChildNode = new DecisionTreeLeafNode( yTrain[split.indexRight[0]] );
         }
         else
         {
            assert( false );
         }
      }
      else
      {
         double yLeft = 0;
         for ( auto it = split.indexLeft.begin(); it != split.indexLeft.end(); ++it )
         {
            yLeft += yTrain[*it];
         }
         yLeft /= split.indexLeft.size();

         double yRight = 0;
         for ( auto it = split.indexRight.begin(); it != split.indexRight.end(); ++it )
         {
            yRight += yTrain[*it];
         }
         yRight /= split.indexRight.size();

         m_leftChildNode = new DecisionTreeLeafNode( yLeft );
         m_rightChildNode = new DecisionTreeLeafNode( yRight );
      }
   }
   else
   {
      /// TODO: should be able to replace by assert?!
      double leafVal = indices.size() > 0? yTrain[indices[0]] : 0;
      m_leftChildNode = new DecisionTreeLeafNode( leafVal );
      m_rightChildNode = new DecisionTreeLeafNode( leafVal );
   }
}



DecisionTreeSplitRef DecisionTreeRefNode::findBestSplit( const Matrix& xTrain, const RealVector& yTrain, const IndexVector& indices, TempArrays& tempArrays )
{
   const size_t nPoints = indices.size();
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
      return DecisionTreeSplitRef( xTrain, yTrain, m_splitFeatureIndex, m_splitValue, indices );
   }

   for ( size_t iFeat = 0; iFeat < nFeats; ++iFeat )
   {
      // const RealVector& featureData = xTrain.getColumn( iFeat );

      const std::vector< RealVector >& rawData = xTrain.getRows();
      RealVector featureData(indices.size());
      size_t insIt = 0;
      for ( auto it = indices.begin(); it != indices.end(); ++it )
      {
         /// TODO: faster if first subselection of rows is made?
         featureData[insIt] = rawData[*it][iFeat];
         ++insIt;
      }

      SortCache sc( featureData );
      const RealVector& sortedFeatureData = sc.applyTo( featureData );
      RealVector&& ySorted = sc.applyTo( applyIndexing(yTrain, indices ) );

      const RealVector& sumStdDev = computeSumStdLeftRight( ySorted, tempArrays );

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

   return DecisionTreeSplitRef( xTrain, yTrain, m_splitFeatureIndex, m_splitValue, indices );
}




} /// namespace MlLib
