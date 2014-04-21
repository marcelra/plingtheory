#include "Neuron.h"

namespace Mva
{

MlpNode::MlpNode() :
   m_activation( 0 )
{}

MlpNode::~MlpNode()
{}

void MlpNode::addSynapse( const Synapse& synapse )
{
   m_synapses.push_back( synapse );
}

std::vector< double* > MlpNode::getWeights()
{
   std::vector< double* > result;
   for ( size_t i = 0; i < m_synapses.size(); ++i )
   {
      result.push_back( &m_synapses[ i ].getWeightRef() );
   }
   return result;
}

Neuron::Neuron() :
   m_bias( 0 )
{}

std::vector< double* > Neuron::getWeights()
{
   std::vector< double* > weights = MlpNode::getWeights();
   weights.push_back( &m_bias );
   return weights;
}


} /// namespace Mva
