#include "DecisionTree.h"

#include "SortCache.h"
#include "Utils.h"
#include "Matrix.h"
#include "Logger.h"
#include "Timer.h"
#include "DecisionTreeNodes.h"

#include <iostream>

namespace MlLib
{

DecisionTree::DecisionTree( size_t maxDepth ) :
   m_maxDepth( maxDepth ),
   m_tree( 0 )
{}

DecisionTree& DecisionTree::fit( const std::vector<RealVector>& xTrain, const RealVector& yTrain )
{
   Logger msg( "DecisionTree" );

   Timer timer( msg, Msg::Info, "DecisionTree::fit" );

   msg << Msg::Info << "Fitting decision tree..." << Msg::EndReq;

   /// TODO: casting it to matrix. Consider changing signature.
   const Matrix& xAsMatrix( xTrain );
   MlLib::TempArrays tempArrays( yTrain.size() );

   m_tree = new DecisionTreeRefNode( 0, m_maxDepth, xAsMatrix, yTrain, Utils::createRange( 0, yTrain.size() ), tempArrays );

   timer.end();

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
