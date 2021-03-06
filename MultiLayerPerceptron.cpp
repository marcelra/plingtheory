#include "MultiLayerPerceptron.h"

#include "RandomNumberGenerator.h"

#include <cmath>
#include <iostream>

namespace Mva
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MultiLayerPerceptron::MultiLayerPerceptron( size_t numInputNodes, size_t numOutputNodes, const std::vector< size_t >& hiddenLayerStructure, bool useBiasNodes ) :
   m_input( numInputNodes ),
   m_output( numOutputNodes ),
   m_deltaEInput( numInputNodes ),
   m_deltaEOutput( numOutputNodes ),
   m_useBiasNodes( useBiasNodes ),
   m_numWeights( 0 )
{
   assert( numInputNodes > 0 && numOutputNodes > 0 );

   /// Initialise neuron quantities.
   for ( auto it = hiddenLayerStructure.begin(); it != hiddenLayerStructure.end(); ++it )
   {
      assert( *it > 0 );
      m_y.push_back( RealVector( *it ) );
      m_dfdy.push_back( RealVector( *it ) );

      m_x.push_back( RealVector( *it ) );
      m_deltaE.push_back( RealVector( *it ) );
   }

   /// Initialise first layer weights.
   size_t numFirstLayerTargetNeurons = m_y.size() > 0 ? m_y[0].size() : m_output.size();
   m_weights.push_back( std::vector< RealVector >( m_input.size(), RealVector( numFirstLayerTargetNeurons, 1 ) ) );

   /// Initialise hidden layer weights.
   for ( int i = 0; i < static_cast< int >( m_y.size() ) - 1; ++i )
   {
      size_t nNeurons = m_y[ i ].size();
      size_t nNeuronsNext = m_y[ i + 1 ].size();
      m_weights.push_back( std::vector< RealVector >( nNeurons, RealVector( nNeuronsNext, 1 ) ) );
   }

   /// Initialse final layer weights.
   if ( hiddenLayerStructure.size() != 0 )
   {
      m_weights.push_back( std::vector< RealVector >( m_y.back().size(), RealVector( m_output.size(), 1 ) ) );
   }

   /// Initialise Bias nodes.
   if ( useBiasNodes )
   {
      for ( size_t i = 0; i < m_x.size(); ++i )
      {
         m_x[ i ].push_back( 1 );
         m_deltaE[ i ].push_back( 0 );
      }

      m_deltaEOutput.push_back( 0 );
      m_input.push_back( 1 );

      for ( size_t iLayer = 0; iLayer < m_weights.size(); ++iLayer )
      {
         m_weights[ iLayer ].push_back( m_weights[ iLayer ][ 0 ] );

         /// Initialise bias weights with 0
         for ( size_t i = 0; i < m_weights[ iLayer ].back().size(); ++i )
         {
            m_weights[ iLayer ].back()[ i ] = 0;
         }
      }
   }

   m_weightDerivatives = m_weights;

   /// Count the number of weights.
   for ( size_t i = 0; i < m_weights.size(); ++i )
   {
      for ( size_t j = 0; j < m_weights[ i ].size(); ++j )
      {
         m_numWeights += m_weights[ i ][ j ].size();
      }
   }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// evaluate
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVector MultiLayerPerceptron::evaluate( const RealVector& x )
{
   /// Assert validity of input.
   assert( !m_useBiasNodes && x.size() == m_input.size() || m_useBiasNodes && x.size() == m_input.size() - 1 );

   /// Set input.
   for ( size_t i = 0; i < x.size(); ++i )
   {
      m_input[ i ] = x[ i ];
   }

   /// Simple forward propagation when there are no hidden layers.
   if ( m_y.size() == 0 )
   {
      propagateForward( m_input, m_output, m_weights[ 0 ] );
   }
   else
   {
      /// Propage from input node to first hidden layer.
      propagateForward( m_input, m_y[ 0 ], m_weights[ 0 ] );
      applyActivationFunc( m_y[ 0 ], m_x[ 0 ] );

      /// Propagate to last hidden layer.
      for ( size_t iHiddenLayer = 0; iHiddenLayer < m_y.size() - 1; ++iHiddenLayer )
      {
         propagateForward( m_x[ iHiddenLayer ], m_y[ iHiddenLayer + 1 ], m_weights[ iHiddenLayer + 1 ] );
         applyActivationFunc( m_y[ iHiddenLayer + 1 ], m_x[ iHiddenLayer + 1 ] );
      }

      /// Propagate to output nodes.
      propagateForward( m_x.back(), m_output, m_weights.back() );
   }

   return m_output;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// calcError
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double MultiLayerPerceptron::calcError( const RealVector& input, const RealVector& target )
{
   const RealVector& response = evaluate( input );

   assert( target.size() == response.size() );

   /// Calculate the error and deltas on the output nodes.
   double error = 0;
   for ( size_t i = 0; i < response.size(); ++i )
   {
      double diff = response[ i ] - target[ i ];
      error += diff * diff;
      m_deltaEOutput[ i ] = diff;
   }
   error *= 0.5;
   return error;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getErrorDerivative
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVector MultiLayerPerceptron::calcErrorAndGradient( const RealVector& input, const RealVector& target, double& error )
{
   /// The current state on the nodes will be used for backpropagation.
   error = calcError( input, target );

   if ( m_deltaE.size() == 0 )
   {
      /// Calculate the gradient components with a single backward propagation if there are no hidden layers.
      propagateBackward( m_deltaEOutput, m_deltaEInput, m_weights[ 0 ] );
      for ( size_t iInput = 0; iInput < m_weights[ 0 ].size(); ++iInput )
      {
         for ( size_t iOutput = 0; iOutput < m_weights[ 0 ][ iInput ].size(); ++iOutput )
         {
            m_weightDerivatives[ 0 ][ iInput ][ iOutput ] = m_input[ iInput ] * m_deltaEOutput[ iOutput ];
         }
      }
   }
   else
   {
      /// Calculate the derivatives of the activation function on all the hidden nodes. This is independent of
      /// backpropagation.
      calcDerivativesActivationFunc();

      /// Propagate output deltas to first hidden node.
      propagateBackward( m_deltaEOutput, m_deltaE.back(), m_weights.back() );
      for ( size_t i = 0; i < m_deltaE.back().size(); ++i )
      {
         m_deltaE.back()[ i ] *= m_dfdy.back()[ i ];
      }

      /// Calculate deltas for all hidden nodes.
      /// delta_nk = f'(y_nk) * sum_i w_nki delta_(n+1)i
      /// propageBackward calculates the sum.
      for ( size_t iHiddenLayer = m_deltaE.size() - 1; iHiddenLayer > 0; --iHiddenLayer )
      {
         propagateBackward( m_deltaE[ iHiddenLayer ], m_deltaE[ iHiddenLayer - 1 ], m_weights[ iHiddenLayer ] );
         for ( size_t iNeuron = 0; iNeuron < m_deltaE[ iHiddenLayer - 1 ].size(); ++iNeuron )
         {
            m_deltaE[ iHiddenLayer - 1 ][ iNeuron ] *= m_dfdy[ iHiddenLayer - 1 ][ iNeuron ];
         }
      }

      /// Calculate the derivatives. dE/dw_nij = x_(n-1)j * delta_i
      for ( size_t iLayer = 0; iLayer < m_weights.size(); ++iLayer )
      {
         const RealVector& sourceNeuronActivations = iLayer != 0 ? m_x[ iLayer - 1 ] : m_input;
         const RealVector& deltaVec = iLayer < m_deltaE.size() ? m_deltaE[ iLayer ] : m_deltaEOutput;
         for ( size_t iSourceNeuron = 0; iSourceNeuron < m_weights[ iLayer ].size(); ++iSourceNeuron )
         {
            for ( size_t iTargetNeuron = 0; iTargetNeuron < m_weights[ iLayer ][ iSourceNeuron ].size(); ++iTargetNeuron )
            {
               m_weightDerivatives[ iLayer ][ iSourceNeuron ][ iTargetNeuron ] = sourceNeuronActivations[ iSourceNeuron ] * deltaVec[ iTargetNeuron ];
            }
         }
      }
   }

   /// @see composeGradient
   return composeGradient();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// propagateFwd
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MultiLayerPerceptron::propagateForward( const RealVector& sourceLayer, RealVector& destLayer, const std::vector< RealVector >& weights )
{
   assert( weights.size() == sourceLayer.size() );

   for ( size_t iDestNeuron = 0; iDestNeuron < destLayer.size(); ++iDestNeuron )
   {
      destLayer[ iDestNeuron ] = 0;
      for ( size_t iSourceNeuron = 0; iSourceNeuron < sourceLayer.size(); ++iSourceNeuron )
      {
         assert( weights[ iSourceNeuron ].size() == destLayer.size() );
         destLayer[ iDestNeuron ] += sourceLayer[ iSourceNeuron ] * weights[ iSourceNeuron ][ iDestNeuron ];
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// propagateBackward
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MultiLayerPerceptron::propagateBackward( const RealVector& sourceLayer, RealVector& destLayer, const std::vector< RealVector >& weights )
{
   assert( weights.size() == destLayer.size() );
   for ( size_t iDestNeuron = 0; iDestNeuron < destLayer.size(); ++iDestNeuron )
   {
      destLayer[ iDestNeuron ] = 0;
      size_t sourceSize = m_useBiasNodes ? sourceLayer.size() - 1 : sourceLayer.size();

      assert( weights[ iDestNeuron ].size() == sourceSize );
      for ( size_t iSourceNeuron = 0; iSourceNeuron < sourceSize; ++iSourceNeuron )
      {
         destLayer[ iDestNeuron ] += sourceLayer[ iSourceNeuron ] * weights[ iDestNeuron ][ iSourceNeuron ];
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// applyActivationFunc
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MultiLayerPerceptron::applyActivationFunc( const RealVector& neuronActivation, RealVector& neuronResponse )
{
   for ( size_t i = 0; i < neuronActivation.size(); ++i )
   {
      neuronResponse[ i ] = tanh( neuronActivation[ i ] );
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// calcDerivativesActivationFunc
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MultiLayerPerceptron::calcDerivativesActivationFunc()
{
   for ( size_t iLayer = 0; iLayer < m_y.size(); ++iLayer )
   {
      for ( size_t iNeuron = 0; iNeuron < m_y[ iLayer ].size(); ++iNeuron )
      {
         /// NB. m_x = f(m_y).
         double tanhY = m_x[ iLayer ][ iNeuron ];
         m_dfdy[ iLayer ][ iNeuron ] = 1 - tanhY * tanhY;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// composeGradient
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVector MultiLayerPerceptron::composeGradient()
{
   RealVector result( getNumWeights() );

   size_t index = 0;
   for ( size_t iLayer = 0; iLayer < m_weightDerivatives.size(); ++iLayer )
   {
      for ( size_t iSource = 0; iSource < m_weightDerivatives[ iLayer ].size(); ++iSource )
      {
         for ( size_t iDest = 0; iDest < m_weightDerivatives[ iLayer ][ iSource ].size(); ++iDest, ++index )
         {
            result[ index ] = m_weightDerivatives[ iLayer ][ iSource ][ iDest ];
         }
      }
   }
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// randomiseWeights
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MultiLayerPerceptron::randomiseWeights( const Interval& interval, size_t seed )
{
   RandomNumberGenerator rng( seed );
   for ( size_t i = 0; i < m_weights.size(); ++i )
   {
      for ( size_t j = 0; j < m_weights[ i ].size(); ++j )
      {
         for ( size_t k = 0; k < m_weights[ i ][ j ].size(); ++k )
         {
            m_weights[ i ][ j ][ k ] = rng.uniform( interval.getMin(), interval.getMax() );
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getWeightReference
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector< double > MultiLayerPerceptron::getWeights()
{
   std::vector< double > result;
   for ( size_t i = 0; i < m_weights.size(); ++i )
   {
      for ( size_t j = 0; j < m_weights[ i ].size(); ++j )
      {
         for ( size_t k = 0; k < m_weights[ i ][ j ].size(); ++k )
         {
            result.push_back( m_weights[ i ][ j ][ k ] );
         }
      }
   }
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setWeights
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MultiLayerPerceptron::setWeights( const std::vector< double >& weights )
{
   size_t l = 0;
   for ( size_t i = 0; i < m_weights.size(); ++i )
   {
      for ( size_t j = 0; j < m_weights[ i ].size(); ++j )
      {
         for ( size_t k = 0; k < m_weights[ i ][ j ].size(); ++k )
         {
            m_weights[ i ][ j ][ k ] = weights[ l ];
            ++l;
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getNumWeights
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t MultiLayerPerceptron::getNumWeights() const
{
   return m_numWeights;
}

} /// namespace Mva
