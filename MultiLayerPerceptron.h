#ifndef MULTILAYERPERCEPTRON_H
#define MULTILAYERPERCEPTRON_H

#include "Neuron.h"
#include "RealVector.h"

namespace Mva
{

/**
 * @class MultiLayerPerceptron
 * @brief MultiLayerPerceptron feed-forward neural network.
 */
class MultiLayerPerceptron
{
   public:
      /**
       * Create network with @param numInputNodes input nodes and @param numOutputNodes output nodes.
       */
      MultiLayerPerceptron( size_t numInputNodes, size_t numOutputNodes );
      /**
       * Destructor
       */
      ~MultiLayerPerceptron();

      /**
       * Add hidden layer with @param numNeurons in front of the output layer.
       */
      void addHiddenLayer( size_t numNeurons );
      /**
       * Connect all layers.
       */
      void build();

      /**
       * Evaluate neural network response for vector @param x.
       */
      RealVector evaluate( const RealVector& x );

      /**
       * Get the number of input nodes (dimensionality of input data vectors).
       */
      size_t getNumInputNodes() const;
      /**
       * Get the number of output nodes.
       */
      size_t getNumOutputNodes() const;

      /**
       * Train the network using the ParticleSwarmOptimiser.
       */
      void trainPso( const std::vector< RealVector >& inputData, const std::vector< RealVector >& outputData );
      /**
       * Train the network using the McmcOptimiser.
       */
      void trainMcmc( const std::vector< RealVector >& inputData, const std::vector< RealVector >& outputData );

      /**
       * Get pointer vector to all weights. Note that if the values pointed are modified, the network is changed.
       */
      std::vector< double* > getWeights();

   private:
      /**
       * Readability typedef.
       */
      typedef std::vector< MlpNode* > NeuronLayer;

   private:
      /**
       * Get neuron layer indexed by @param index. First layer, index 0, is point to the layer of input nodes.
       */
      NeuronLayer& getNeuronLayer( size_t index );
      /**
       * Get the number of neuron layers. This is the number of hidden layers + 2 (for input and output layers).
       */
      size_t getNumNeuronLayers() const;

   private:
      NeuronLayer                   m_inputLayer;        //! Input layer.
      NeuronLayer                   m_outputLayer;       //! Output layer.
      std::vector< NeuronLayer >    m_neurons;           //! Hidden layers.
};

} /// namespace Mva

#endif // MULTILAYERPERCEPTRON_H
