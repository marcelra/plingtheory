#include "RootMlp.h"

#include "TMultiLayerPerceptron.h"
#include "TTree.h"

namespace Mva
{

RootMlp::RootMlp( const std::string& networkLayout ) :
   m_networkLayout( networkLayout )
{}

void RootMlp::train( const std::vector< RealVector >& inputData, const std::vector< RealVector >& outputData )
{
   TTree* trainSetTree = getTreeFromTrainData( inputData, outputData );
   m_mlp.reset( new TMultiLayerPerceptron( m_networkLayout.c_str(), trainSetTree ) );
   // m_mlp->Train( 10, "text" );
   m_mlp.release();
}

TTree* RootMlp::getTreeFromTrainData( const std::vector< RealVector >& inputData, const std::vector< RealVector >& outputData )
{
   TTree* tree = new TTree( "TrainData", "TrainData" );

   size_t nInputs = inputData[ 0 ].size();
   size_t nOutputs = outputData[ 0 ].size();

   RealVector inputHelper( nInputs );
   RealVector outputHelper( nOutputs );

   for ( size_t iInput = 0; iInput < nInputs; ++iInput )
   {
      TString branchName = "i";
      branchName += iInput;
      tree->Branch( branchName, &inputHelper[ iInput ], branchName + "/D" );
   }

   for ( size_t iOutput = 0; iOutput < nOutputs; ++iOutput )
   {
      TString branchName = "o";
      branchName += iOutput;
      tree->Branch( branchName, &outputHelper[ iOutput ], branchName + "/D" );
   }

   for ( size_t iSample = 0; iSample < inputData.size(); ++iSample )
   {
      for ( size_t iInput = 0; iInput < nInputs; ++iInput )
      {
         inputHelper[ iInput ] = inputData[ iSample ][ iInput ];
      }
      for ( size_t iOutput = 0; iOutput < nOutputs; ++iOutput )
      {
         outputHelper[ iOutput ] = outputData[ iSample ][ iOutput ];
      }
      tree->Fill();
   }

   return tree;
}

RealVector RootMlp::evaluate( const RealVector& x )
{
   RealVector xNonConst( x );
   double r = m_mlp->Evaluate( 0, reinterpret_cast< Double_t* >( &xNonConst[ 0 ] ) );
   RealVector output( 1, r );
   return output;
}

} /// namespace Mva
