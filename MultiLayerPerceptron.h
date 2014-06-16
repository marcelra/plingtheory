#ifndef MULTILAYERPERCEPTRON_H
#define MULTILAYERPERCEPTRON_H

#include <RealVector.h>
#include "Interval.h"

namespace Mva
{

/**
 * @class MultiLayerPerceptron
 * @brief MultiLayerPerceptron artificial neural network with backpropagation.
 */
class MultiLayerPerceptron
{
   public:
      /**
       * Create a MultiLayerPerceptron with @param numInputNodes input nodes and @param numOutputNodes output nodes.
       * The @param hiddenLayerStructure should contain the number of hidden neurons per layer. If @param useBiasNodes
       * is toggled, every hidden layer and the input layer get an additional bias node.
       */
      MultiLayerPerceptron( size_t numInputNodes, size_t numOutputNodes, const std::vector< size_t >& hiddenLayerStructure, bool useBiasNodes = true );

      /**
       * Get the NN response for vector @param x.
       */
      RealVector evaluate( const RealVector& x );

      /**
       * Retrieve the weights.
       */
      std::vector< double > getWeights();
      /**
       * Set weights.
       */
      void setWeights( const std::vector< double >& weights );
      /**
       * Randomise the weights.
       */
      void randomiseWeights( const Interval& interval, size_t seed );
      /**
       * Get the number of weights
       */
     size_t getNumWeights() const;

   public:
      /**
       * Calculate the error and gradient (@param[out] error and @return) for a sample @param input and
       * desired output @param target.
       */
      RealVector calcErrorAndGradient( const RealVector& input, const RealVector& target, double& error );

      /**
       * Calculate the error for an individual sample.
       */
      double calcError( const RealVector& input, const RealVector& target );

      /**
       * Propagate neuron response from @param sourceLayer to activate @param destLayer.
       */
      void propagateForward( const RealVector& sourceLayer, RealVector& destLayer, const std::vector< RealVector >& weights );
      /**
       * Backpropagation. The weights should not be transposed for this operation (i.e. use the feedforward weights
       * representation for the matching layer).
       */
      void propagateBackward( const RealVector& sourceLayer, RealVector& destLayer, const std::vector< RealVector >& weights );

      /**
       * Apply activation function on @param neuronActivation and store result in @param[out] neuronResponse.
       * The neuronResponse vector should be initialised. This method is applied to compute x from y.
       */
      void applyActivationFunc( const RealVector& neuronActivation, RealVector& neuronResponse );

      /**
       * Calculate all derivatives df/dy on all neurons.
       */
      void calcDerivativesActivationFunc();

   private:
      /**
       * Flatten m_weightDerivatives into a single weight vector.
       */
      RealVector composeGradient();

   private:
      RealVector						              m_input;		         //! Input nodes values.
      RealVector						              m_output;	         	//! Output nodes values.
      std::vector< RealVector >	              m_y;			         //! Hidden layer neuron activations. First index layer, second index neuron in layer.
      std::vector< RealVector >	              m_x;			         //! Hidden layer neuron response values. First index layers, second index neuron in layer.

      std::vector< RealVector >					  m_deltaE;				   //! Backpropagation delta values (dE/dy) on hidden nodes.
      RealVector              					  m_deltaEInput;   		//! Backpropagation delta values on input nodes.
      RealVector              					  m_deltaEOutput;  		//! Backpropagation delta values on output nodes.
      std::vector< RealVector >					  m_dfdy;				   //! Derivative of activation function on hidden nodes.
      std::vector< std::vector< RealVector > > m_weights;			   //! All weights. Indexing: layer, source node, dest node.

      std::vector< std::vector< RealVector > > m_weightDerivatives;	//! Derivates of all weights.
      bool												  m_useBiasNodes;			//! Indicator whether or not to include bias nodes.
      size_t											  m_numWeights;		   //! The number of weights in this MLP.
};

} /// namespace Mva

#endif // MULTILAYERPERCEPTRON_H
