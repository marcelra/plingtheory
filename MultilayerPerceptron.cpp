#include "MultilayerPerceptron.h"

#include <cmath>

namespace Mva
{

Synapse::Synapse( Neuron* to, double weight ) :
   m_to( to ),
   m_weight( weight )
{}

void Synapse::fire( double value )
{
   m_to->excite( value * m_weight );
}

void Neuron::addConncetion( const Synapse& synapse )
{
   m_connections.push_back( synapse );
}

Network::Network( size_t numInputNeurons, size_t numOutputNeurons ) :
   m_inputNeurons( numInputNeurons ),
   m_outputNeurons( numOutputNeurons )
{}

Network::~Network()
{}

void Network::addHiddenLayer( size_t numNeurons )
{
   m_hiddenLayers.push_back( std::vector< Neuron >( numNeurons ) );
}

void Network::connectNeurons( Neuron* from, Neuron* to, double weight )
{
   from->addConncetion( Synapse( to, weight ) );
}

void Network::connectLayers( NeuronLayer& layer0, NeuronLayer& layer1 )
{
   for ( size_t iNeuron0 = 0; iNeuron0 < layer0.size(); ++iNeuron0 )
   {
      Neuron* neuronFrom = &layer0[ iNeuron0 ];
      for ( size_t iNeuron1 = 0; iNeuron1 < layer1.size(); ++iNeuron1 )
      {
         Neuron* neuronTo = &layer1[ iNeuron1 ];
         connectNeurons( neuronFrom, neuronTo, 1 );
      }
   }
}

size_t Network::getNumLayers() const
{
   return m_hiddenLayers.size() + 2;
}

Network::NeuronLayer& Network::getNeuronLayer( size_t index )
{
   if ( index == 0 )
   {
      return m_inputNeurons;
   }
   else if ( index < getNumLayers() - 1 )
   {
      return m_hiddenLayers[ index - 1 ];
   }
   else
   {
      assert( index == getNumLayers() - 1 );
      return m_outputNeurons;
   }
}

void Network::build()
{
   for ( size_t iLayer = 0; iLayer < getNumLayers() - 1; ++iLayer )
   {
      connectLayers( getNeuronLayer( iLayer ), getNeuronLayer( iLayer + 1 ) );
   }
}

MultilayerPerceptron::MultilayerPerceptron( const Network& network, const AlgorithmBase* parent ) :
   AlgorithmBase( "MultilayerPerceptron", parent ),
   m_network( network )
{
   m_network.build();
}

Network& MultilayerPerceptron::getNetwork()
{
   return m_network;
}

void Neuron::excite( double weight )
{
   m_in += weight;
}

void Neuron::relax()
{
   m_in = 0;
   m_out = 0;
}

double Neuron::activationFunc( double x ) const
{
   return tanh( x );
}

void Neuron::fire()
{
   m_out = activationFunc( m_in );

   for ( size_t iConn = 0; iConn < m_connections.size(); ++iConn )
   {
      m_connections[ iConn ].fire( m_in );
   }
}

RealVector Network::eval( const RealVector& input )
{
   relax();
   for ( size_t iInput = 0; iInput < input.size(); ++iInput )
   {
      m_inputNeurons[ iInput ].excite( input[ iInput ] );
   }
   for ( size_t iLayer = 0; iLayer < getNumLayers(); ++iLayer )
   {
      NeuronLayer& layer = getNeuronLayer( iLayer );
      for ( size_t iNeuron = 0; iNeuron < layer.size(); ++iNeuron )
      {
         layer[ iNeuron ].fire();
      }
   }

   RealVector result( m_outputNeurons.size() );
   for ( size_t iOutput = 0; iOutput < result.size(); ++iOutput )
   {
      result[ iOutput ] = m_outputNeurons[ iOutput ].getValueOut();
   }
   return result;
}

void Network::relax()
{
   for ( size_t iNeuronLayer = 0; iNeuronLayer < getNumLayers(); ++iNeuronLayer )
   {
      NeuronLayer& layer = getNeuronLayer( iNeuronLayer );
      for ( size_t iNeuron = 0; iNeuron < layer.size(); ++iNeuron )
      {
         layer[ iNeuron ].relax();
      }
   }
}

double Neuron::getValueOut() const
{
   return m_out;
}

std::vector< double* > Network::getWeights()
{
   std::vector< double* > weightVector;
   for ( size_t iNeuronLayer = 0; iNeuronLayer < getNumLayers(); ++iNeuronLayer )
   {
      NeuronLayer& layer = getNeuronLayer( iNeuronLayer );
      for ( size_t iNeuron = 0; iNeuron < layer.size(); ++iNeuron )
      {
         Neuron& neuron = layer[ iNeuron ];
         std::vector< Synapse >& synapses = neuron.getConnections();
         for ( size_t iSynapse = 0; iSynapse < synapses.size(); ++iSynapse )
         {
            weightVector.push_back( &synapses[ iSynapse ].getWeightRef() );
         }
      }
   }
   return weightVector;
}

} /// namespace Mva
