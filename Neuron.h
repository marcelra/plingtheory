#ifndef NEURON_H
#define NEURON_H

#include "Synapse.h"

#include <cmath>
#include <vector>

/// TODO: temp
#include <iostream>

namespace Mva
{

class MlpNode
{
   public:
      MlpNode();
      virtual ~MlpNode();

      void addSynapse( const Synapse& synapse );

      void activate( double value );
      virtual std::vector< double* > getWeights();

   protected:
      double                  m_activation;
      std::vector< Synapse >  m_synapses;
};

class OutputNode : public MlpNode
{
   public:
      double getActivation();
      void resetActivation();
};

class Neuron : public MlpNode
{
   public:
      Neuron();

      virtual double activationFunc( double value );

      void fire();
      std::vector< double* > getWeights();

   protected:
      double   m_bias;
};

class InputNode : public Neuron
{
   public:
      virtual double activationFunc( double value );
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods Neuron
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void MlpNode::activate( double value )
{
   m_activation += value;
}

inline void Neuron::fire()
{
   double value = activationFunc( m_activation );
   for ( size_t iSynapse = 0; iSynapse < m_synapses.size(); ++iSynapse )
   {
      m_synapses[ iSynapse ].fire( value );
   }
   m_activation = 0;
}

inline double Neuron::activationFunc( double value )
{
   return tanh( m_bias + value );
}

inline double OutputNode::getActivation()
{
   return m_activation;
}

inline void OutputNode::resetActivation()
{
   m_activation = 0;
}

inline double InputNode::activationFunc( double value )
{
   return value + m_bias;
}

} /// namespace Mva

#endif // NEURON_H
