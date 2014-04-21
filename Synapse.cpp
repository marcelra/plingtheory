#include "Synapse.h"

#include "Neuron.h"

namespace Mva
{

Synapse::Synapse( MlpNode* to, double weight ) :
   m_neuron( to ),
   m_weight( weight )
{}

void Synapse::fire( double value )
{
   m_neuron->activate( value * m_weight );
}

double& Synapse::getWeightRef()
{
   return m_weight;
}

} /// namespace Mva
