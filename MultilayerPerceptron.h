#ifndef MULTILAYERPERCEPTRON_H
#define MULTILAYERPERCEPTRON_H

#include "Neuron.h"
#include "RealVector.h"

namespace Mva
{

class MultilayerPerceptron
{
   public:
      MultilayerPerceptron( size_t numInputNodes, size_t numOutputNodes );
      ~MultilayerPerceptron();

      void addHiddenLayer( size_t numNeurons );
      void build();

      RealVector evaluate( const RealVector& x );

      size_t getNumInputNodes() const;
      size_t getNumOutputNodes() const;

      void train( const std::vector< RealVector >& inputData, const std::vector< RealVector >& outputData );
      std::vector< double* > getWeights();

   private:
      typedef std::vector< MlpNode* > NeuronLayer;

   private:
      NeuronLayer& getNeuronLayer( size_t index );
      size_t getNumNeuronLayers() const;

   private:
      NeuronLayer                   m_inputLayer;
      NeuronLayer                   m_outputLayer;
      std::vector< NeuronLayer >    m_neurons;
};

} /// namespace Mva

#endif // MULTILAYERPERCEPTRON_H
