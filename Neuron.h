#ifndef NEURON_H
#define NEURON_H

#include "Synapse.h"

#include <cstddef>
#include <vector>

namespace Mva
{

/**
 * @class MlpNode.
 * @brief Class representing the common base for input-, output- and hidden layer neurons used internally in MultiLayerPerceptron.
 */
class MlpNode
{
   public:
      /**
       * Constructor.
       */
      MlpNode();
      /**
       * Virtual destructor.
       */
      virtual ~MlpNode();

      /**
       * Add a connection/synapse.
       */
      void addSynapse( const Synapse& synapse );

      /**
       * Add weight @param value to current activation.
       */
      void activate( double value );

      /**
       * Get weights of outgoing synapses.
       */
      virtual std::vector< double* > getWeights();

   protected:
      double                  m_activation;
      std::vector< Synapse >  m_synapses;
};

/**
 * @class OutputNode
 * @brief Class describing a MultiLayerPerceptron output node.
 */
class OutputNode : public MlpNode
{
   public:
      /**
       * Get the current activiation (sum of weights).
       */
      double getActivation();
      /**
       * Reset the current activiation.
       */
      void resetActivation();
};

/**
 * @class Neuron
 * @brief Class describing a hidden layer MultiLayerPerceptron neuron.
 */
class Neuron : public MlpNode
{
   public:
      /**
       * Constructor. Initialises the bias to zero.
       */
      Neuron();

      /**
       * Evaluate the activationFunc (usually a sigmoid or tanh).
       */
      virtual double activationFunc( double value );

      /**
       * Excite all connected neurons via the attached synapses.
       */
      void fire();
      /**
       * Get the weights (@see MlpNode::getWeights). This bias weight is appended at the end of the synapse weights vector.
       */
      std::vector< double* > getWeights();

   protected:
      double   m_bias;        //! Neuron bias value.
};

/**
 * @class InputNode
 * @brief The InputNode is similar to the MlpNode, in that it has a bias correction. The activiation function is linear.
 */
class InputNode : public Neuron
{
   public:
      /**
       * Linear activation function for input nodes.
       */
      virtual double activationFunc( double value );
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods MlpNode
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void MlpNode::activate( double value )
{
   m_activation += value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods Neuron
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Neuron::fire()
{
   double value = activationFunc( m_activation );
   for ( size_t iSynapse = 0; iSynapse < m_synapses.size(); ++iSynapse )
   {
      m_synapses[ iSynapse ].fire( value );
   }
   m_activation = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methdos OutputNode
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double OutputNode::getActivation()
{
   return m_activation;
}

inline void OutputNode::resetActivation()
{
   m_activation = 0;
}

} /// namespace Mva

#endif // NEURON_H
