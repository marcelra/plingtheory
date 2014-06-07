#ifndef MLP2_H
#define MLP2_H

#include <RealVector.h>
#include "Interval.h"

namespace Mva
{

class Mlp2
{
   public:
      Mlp2( size_t numInputNodes, size_t numOutputNodes, const std::vector< size_t >& hiddenLayerStructure, bool useBiasNodes = true );

      RealVector evaluate( const RealVector& x );

      std::vector< double > getWeights();
      void setWeights( const std::vector< double >& weights );
      void randomiseWeights( const Interval& interval );

   /// TODO: will be private.
   public:
      void calcErrorAndGradient( const RealVector& input, const RealVector& target, double& error, RealVector& gradient );

   private:
      void propagateForward( const RealVector& sourceLayer, RealVector& destLayer, const std::vector< RealVector >& weights );
      void propagateBackward( const RealVector& sourceLayer, RealVector& destLayer, const std::vector< RealVector >& weights );

      void applyActivationFunc( const RealVector& neuronActivation, RealVector& neuronResponse );
      void calcDerivativesActivationFunc();

   private:
      void composeGradient( RealVector& gradient );

   private:
      RealVector						              m_input;		         //! Input nodes values.
      RealVector						              m_output;	         	//! Output nodes values.
      std::vector< RealVector >	              m_y;			         //! Hidden layer neuron activations. First index layer, second index neuron in layer.
      std::vector< RealVector >	              m_x;			         //! Hidden layer neuron values. First index layers, second index neuron in layer.

      std::vector< RealVector >					  m_deltaE;				   //! Backpropagation delta values (dE/dy) on hidden nodes.
      RealVector              					  m_deltaEInput;   		//! Backpropagation delta values on input nodes.
      RealVector              					  m_deltaEOutput;  		//! Backpropagation delta values on output nodes.
      std::vector< RealVector >					  m_dfdy;				   //! Derivative of activation function on hidden nodes.
      std::vector< std::vector< RealVector > > m_weights;			   //! All weights. Indexing: layer, source node, dest node.

      std::vector< std::vector< RealVector > > m_weightDerivatives;	//! Derivates of all weights.
      bool												  m_useBiasNodes;			//! Indicator whether or not to include bias nodes.
};

#include "IObjectiveFunction.h"

class MlpPerturbative : public Math::IObjectiveFunction
{
   public:
      MlpPerturbative( Mlp2& mlp2, RealVector& eval, RealVector& target ) :
         m_mlp( mlp2 ),
         m_eval( eval ),
         m_target( target )
      {}

      double evaluate( const RealVector& weights ) const
      {
         double error;
         RealVector grad;
         m_mlp.setWeights( weights );
         m_mlp.calcErrorAndGradient( m_eval, m_target, error, grad );
         return error;
      }

      size_t getNumParameters() const
      {
         return m_mlp.getWeights().size();
      }

   private:
      Mlp2&					m_mlp;
      RealVector			m_eval;
      RealVector			m_target;
};

} /// namespace Mva

#endif // MLP2_H
