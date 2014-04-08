#ifndef MULTILAYERPERCEPTRON_H
#define MULTILAYERPERCEPTRON_H

#include <cstddef>
#include <vector>

#include "AlgorithmBase.h"
#include "RealVector.h"

namespace Mva
{

class Neuron;

class Synapse
{
   public:
      Synapse( Neuron* to, double weight );

      void fire( double value );

      double& getWeightRef()
      {
         return m_weight;
      }

   private:
      Neuron*        m_to;
      double         m_weight;
};

class Neuron
{
   public:
      virtual ~Neuron() {}

      void addConncetion( const Synapse& synapse );

      void excite( double weight );
      void relax();
      void fire();

      double getValueOut() const;

      virtual double activationFunc( double x ) const;

      std::vector< Synapse >& getConnections()
      {
         return m_connections;
      }

   private:
      std::vector< Synapse > m_connections;

   private:
      double                 m_in;
      double                 m_out;
};

class Network
{
   public:
      Network( size_t numInputNeurons, size_t numOutputNeurons );
      virtual ~Network();

      void addHiddenLayer( size_t numNeurons );
      void build();

      RealVector eval( const RealVector& input );
      void relax();

      std::vector< double* > getWeights();

   private:
      typedef std::vector< Neuron > NeuronLayer;

      void connectNeurons( Neuron* from, Neuron* to, double weight );
      void connectLayers( NeuronLayer& layer1, NeuronLayer& layer2 );

   private:
      size_t getNumLayers() const;
      NeuronLayer& getNeuronLayer( size_t index );


   private:
      NeuronLayer      m_inputNeurons;
      NeuronLayer      m_outputNeurons;
      std::vector< NeuronLayer > m_hiddenLayers;
};

class MultilayerPerceptron : public AlgorithmBase
{
   public:
      MultilayerPerceptron( const Network& network, const AlgorithmBase* parent );

      Network& getNetwork();

   private:
      Network     m_network;
};

} /// namespace Mva

#endif // MULTILAYERPERCEPTRON_H
