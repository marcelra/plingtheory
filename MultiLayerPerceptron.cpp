#include "MultiLayerPerceptron.h"

#include "IObjectiveFunction.h"
#include "Logger.h"
#include "McmcOptimiser.h"
#include "ParticleSwarmOptimiser.h"
#include "SortCache.h"

#include <cmath>
#include <iostream>

namespace
{

class ErrorObjective : public Math::IObjectiveFunction
{
   public:
      ErrorObjective( Mva::MultiLayerPerceptron& network, const std::vector< RealVector >& inputData, const std::vector< RealVector >& outputData );

      double evaluate( const RealVector& x ) const;
      size_t getNumParameters() const;

      double calcSumSquaredError( const RealVector& xMeasured, const RealVector& xTruth ) const;

   private:
      Mva::MultiLayerPerceptron& m_network;
      std::vector< double* >     m_weights;
      const std::vector< RealVector >& m_inputData;
      const std::vector< RealVector >& m_outputData;
};

ErrorObjective::ErrorObjective( Mva::MultiLayerPerceptron& network, const std::vector< RealVector >& inputData, const std::vector< RealVector >& outputData  ) :
   m_network( network ),
   m_weights( m_network.getWeights() ),
   m_inputData( inputData ),
   m_outputData( outputData )
{}

double ErrorObjective::evaluate( const RealVector& x ) const
{
   /// This implicitly modifies the network weights.
   for ( size_t i = 0; i < getNumParameters(); ++i )
   {
      *m_weights[ i ] = x[ i ];
   }

   double error = 0;
   for ( size_t iInputData = 0; iInputData < m_inputData.size(); ++iInputData )
   {
      const RealVector& output = m_network.evaluate( m_inputData[ iInputData ] );
      error += calcSumSquaredError( output, m_outputData[ iInputData ] );
   }
   return -sqrt( error );
}

double ErrorObjective::calcSumSquaredError( const RealVector& xMeasured, const RealVector& xTruth ) const
{
   double diffSquared = 0;
   for ( size_t i = 0; i < xMeasured.size(); ++i )
   {
      double delta = xMeasured[ i ] - xTruth[ i ];
      diffSquared += delta * delta;
   }
   return diffSquared;
}

size_t ErrorObjective::getNumParameters() const
{
   return m_weights.size();
}

} /// anonymous namespace

namespace Mva
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MultiLayerPerceptron::MultiLayerPerceptron( size_t numInputNodes, size_t numOutputNodes ) :
   m_inputLayer( numInputNodes, 0 ),
   m_outputLayer( numOutputNodes, 0 ),
   m_neurons()
{
   for ( size_t iInputNode = 0; iInputNode < numInputNodes; ++iInputNode )
   {
      m_inputLayer[ iInputNode ] = new InputNode();
   }
   for ( size_t iOutputNode = 0; iOutputNode < numOutputNodes; ++iOutputNode )
   {
      m_outputLayer[ iOutputNode ] = new OutputNode();
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MultiLayerPerceptron::~MultiLayerPerceptron()
{
   for ( size_t i = 0; i < m_inputLayer.size(); ++i )
   {
      delete m_inputLayer[ i ];
   }
   for ( size_t i = 0; i < m_outputLayer.size(); ++i )
   {
      delete m_outputLayer[ i ];
   }
   for ( size_t iLayer = 0; iLayer < m_neurons.size(); ++iLayer )
   {
      for ( size_t iNeuron = 0; iNeuron < m_neurons[ iLayer ].size(); ++iNeuron )
      {
         delete m_neurons[ iLayer ][ iNeuron ];
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// addHiddenLayer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MultiLayerPerceptron::addHiddenLayer( size_t numNeurons )
{
   NeuronLayer newNeuronLayer;
   for ( size_t iNeuron = 0; iNeuron < numNeurons; ++iNeuron )
   {
      newNeuronLayer.push_back( new Neuron() );
   }
   m_neurons.push_back( newNeuronLayer );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// build
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MultiLayerPerceptron::build()
{
   for ( size_t iLayer = 0; iLayer < getNumNeuronLayers() - 1; ++iLayer )
   {
      NeuronLayer& layer0 = getNeuronLayer( iLayer );
      NeuronLayer& layer1 = getNeuronLayer( iLayer + 1 );
      for ( size_t iNeuron0 = 0; iNeuron0 < layer0.size(); ++iNeuron0 )
      {
         for ( size_t iNeuron1 = 0; iNeuron1 < layer1.size(); ++iNeuron1 )
         {
            layer0[ iNeuron0 ]->addSynapse( Synapse( layer1[ iNeuron1 ], 1 ) );
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// evaluate
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVector MultiLayerPerceptron::evaluate( const RealVector& x )
{
   assert( x.size() == getNumInputNodes() );

   for ( size_t i = 0; i < x.size(); ++i )
   {
      m_inputLayer[ i ]->activate( x[ i ] );
   }

   for ( size_t iNeuronLayer = 0; iNeuronLayer < getNumNeuronLayers() - 1; ++iNeuronLayer )
   {
      NeuronLayer& layer = getNeuronLayer( iNeuronLayer );
      for ( size_t iNeuron = 0; iNeuron < layer.size(); ++iNeuron )
      {
         static_cast< Neuron* >( layer[ iNeuron ] )->fire();
      }
   }

   RealVector result( getNumOutputNodes() );
   for ( size_t iOutputNeuron = 0; iOutputNeuron < m_outputLayer.size(); ++iOutputNeuron )
   {
      OutputNode* outputNeuron = static_cast< OutputNode* >( m_outputLayer[ iOutputNeuron ] );
      result[ iOutputNeuron ] = outputNeuron->getActivation();
      outputNeuron->resetActivation();
   }

   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getNumInputNodes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t MultiLayerPerceptron::getNumInputNodes() const
{
   return m_inputLayer.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getNumOutputNodes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t MultiLayerPerceptron::getNumOutputNodes() const
{
   return m_outputLayer.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getNeuronLayer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MultiLayerPerceptron::NeuronLayer& MultiLayerPerceptron::getNeuronLayer( size_t index )
{
   if ( index == 0 )
   {
      return m_inputLayer;
   }
   else if ( index <= m_neurons.size() )
   {
      return m_neurons[ index - 1 ];
   }
   else
   {
      assert( index == m_neurons.size() + 1 );
      return m_outputLayer;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getNumNeuronLayers
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t MultiLayerPerceptron::getNumNeuronLayers() const
{
   return m_neurons.size() + 2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// trainPso
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MultiLayerPerceptron::trainPso( const std::vector< RealVector >& inputData, const std::vector< RealVector >& outputData )
{
   std::vector< double* > weightRefVec = getWeights();

   RealVector startValVec( weightRefVec.size() );
   for ( size_t i = 0; i < weightRefVec.size(); ++i )
   {
      startValVec[ i ] = 0; // *weightRefVec[ i ];
   }

   ::ErrorObjective errorFunc( *this, inputData, outputData );
   assert( false );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// trainMcmc
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MultiLayerPerceptron::trainMcmc( const std::vector< RealVector >& inputData, const std::vector< RealVector >& outputData )
{
   Logger msg( "MultiLayerPerceptron" );
   msg << Msg::Verbose << "Training neural network." << Msg::EndReq;

   std::vector< double* > weightRefVec = getWeights();

   RealVector startValVec( weightRefVec.size() );
   for ( size_t i = 0; i < weightRefVec.size(); ++i )
   {
      startValVec[ i ] = 0; // *weightRefVec[ i ];
   }

   ::ErrorObjective errorFunc( *this, inputData, outputData );
   Math::McmcOptimiser mcmcOpt( errorFunc );

   size_t numNets = startValVec.size();
   std::vector< RealVector > startValues( numNets );
   for ( size_t iNet = 0; iNet < numNets; ++iNet )
   {
      startValVec[ iNet ] = 0;
      startValues[ iNet ] = startValVec;
      startValVec[ iNet ] = 1;
   }

   size_t numIter = 2000;

   mcmcOpt.setStartValues( startValues );
   mcmcOpt.setStepSize( 1 );
   mcmcOpt.setNumIterations( numIter );
   mcmcOpt.setBurninSkip( numIter - 10 );

   Math::RealVectorEnsemble solutions = mcmcOpt.solve();

   RealVector solutionError( solutions.size() );

   for ( size_t iSolution = 0; iSolution < solutions.size(); ++iSolution )
   {
      solutionError[ iSolution ] = errorFunc.evaluate( solutions[ iSolution ] );
   }

   SortCache sortCache( solutionError );

   for ( size_t iSolution = 0; iSolution < sortCache.getSize(); ++iSolution )
   {
      size_t index = sortCache.getSortedIndex( iSolution );
      msg << Msg::Info << "Solution " << index << ": error = " << errorFunc.evaluate( solutions[ index ] ) << Msg::EndReq; // << ", vec = " << solutions[ index ] << Msg::EndReq;
   }

   size_t bestIndex = sortCache.getReverseSortedIndex( 0 );
   RealVector bestSolution = solutions[ bestIndex ];

   /// Set the neural network weights
   for ( size_t iWeight = 0; iWeight < weightRefVec.size(); ++iWeight )
   {
      *weightRefVec[ iWeight ] = bestSolution[ iWeight ];
   }

   for ( size_t i = 0; i < weightRefVec.size(); ++i )
   {
      msg << Msg::Info << "weights[ " << i << " ] = " << *weightRefVec[ i ] << Msg::EndReq;
   }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getWeights
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector< double* > MultiLayerPerceptron::getWeights()
{
   std::vector< double* > weights;

   /// Input weights.
   for ( size_t iInput = 0; iInput < m_inputLayer.size(); ++iInput )
   {
      const std::vector< double* >& synapseWeights = m_inputLayer[ iInput ]->getWeights();
      weights.insert( weights.end(), synapseWeights.begin(), synapseWeights.end() );
   }

   /// Hidden layers.
   for ( size_t iLayer = 0; iLayer < m_neurons.size(); ++iLayer )
   {
      for ( size_t iNeuron = 0; iNeuron < m_neurons[ iLayer ].size(); ++iNeuron )
      {
         const std::vector< double* >& neuronWeights = m_neurons[ iLayer ][ iNeuron ]->getWeights();
         weights.insert( weights.end(), neuronWeights.begin(), neuronWeights.end() );
      }
   }

   return weights;
}

} /// namespace Mva
