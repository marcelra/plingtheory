#include "Mlp2.h"

#include "RandomNumberGenerator.h"

#include <cmath>
#include <iostream>

namespace Mva
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Mlp2::Mlp2( size_t numInputNodes, size_t numOutputNodes, const std::vector< size_t >& hiddenLayerStructure ) :
   m_input( numInputNodes ),
   m_output( numOutputNodes ),
   m_deltaEInput( numInputNodes ),
   m_deltaEOutput( numOutputNodes )
{

   /// Initialise neuron quantities
   for ( auto it = hiddenLayerStructure.begin(); it != hiddenLayerStructure.end(); ++it )
   {
      m_y.push_back( RealVector( *it ) );
      m_x.push_back( RealVector( *it ) );
      m_dfdy.push_back( RealVector( *it ) );
      m_deltaE.push_back( RealVector( *it ) );
   }

   /// Initialise first layer weights
   size_t numFirstLayerTargetNeurons = m_y.size() > 0 ? m_y[0].size() : m_output.size();
   m_weights.push_back( std::vector< RealVector >( m_input.size(), RealVector( numFirstLayerTargetNeurons, 1 ) ) );

   /// Initialise hidden layer weights
   for ( int i = 0; i < static_cast< int >( m_y.size() ) - 1; ++i )
   {
      size_t nNeurons = m_y[ i ].size();
      size_t nNeuronsNext = m_y[ i + 1 ].size();
      m_weights.push_back( std::vector< RealVector >( nNeurons, RealVector( nNeuronsNext, 1 ) ) );
   }

   /// Initialse final layer weights
   if ( hiddenLayerStructure.size() != 0 )
   {
      m_weights.push_back( std::vector< RealVector >( m_y.back().size(), RealVector( m_output.size(), 1 ) ) );
   }

   m_weightDerivatives = m_weights;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// evaluate
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVector Mlp2::evaluate( const RealVector& x )
{
   assert( x.size() == m_input.size() );

   m_input = x;

   /// Input scaling and offset (TODO)

   /// No hidden layers
   if ( m_y.size() == 0 )
   {
      propagateForward( m_input, m_output, m_weights[ 0 ] );
   }
   else
   {
      calcDerivativesActivationFunc();

      propagateForward( m_input, m_y[ 0 ], m_weights[ 0 ] );

      applyActivationFunc( m_y[ 0 ], m_x[ 0 ] );

      for ( size_t iHiddenLayer = 0; iHiddenLayer < m_y.size() - 1; ++iHiddenLayer )
      {
         propagateForward( m_x[ iHiddenLayer ], m_y[ iHiddenLayer + 1 ], m_weights[ iHiddenLayer + 1 ] );
         applyActivationFunc( m_y[ iHiddenLayer + 1 ], m_x[ iHiddenLayer + 1 ] );
      }
      propagateForward( m_x.back(), m_output, m_weights.back() );

   }

   /// Output scaling (TODO)
   return m_output;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getErrorDerivative
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mlp2::calcErrorAndGradient( const RealVector& input, const RealVector& target, double& error, RealVector& gradient )
{
   const RealVector& response = evaluate( input );

   assert( target.size() == response.size() );

   error = 0;
   for ( size_t i = 0; i < response.size(); ++i )
   {
      double diff = response[ i ] - target[ i ];
      error += diff * diff;
      m_deltaEOutput[ i ] = diff;
   }
   error *= 0.5;

   if ( m_deltaE.size() == 0 )
   {
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
      calcDerivativesActivationFunc();

      propagateBackward( m_deltaEOutput, m_deltaE.back(), m_weights.back() );
      for ( size_t i = 0; i < m_deltaE.back().size(); ++i )
      {
         m_deltaE.back()[ i ] *= m_dfdy.back()[ i ];
      }

      for ( size_t iHiddenLayer = m_deltaE.size() - 1; iHiddenLayer > 0; --iHiddenLayer )
      {
         propagateBackward( m_deltaE[ iHiddenLayer ], m_deltaE[ iHiddenLayer - 1 ], m_weights[ iHiddenLayer ] );
         for ( size_t iNeuron = 0; iNeuron < m_deltaE[ iHiddenLayer - 1 ].size(); ++iNeuron )
         {
            m_deltaE[ iHiddenLayer - 1 ][ iNeuron ] *= m_dfdy[ iHiddenLayer - 1 ][ iNeuron ];
         }

      }

      for ( size_t iNeuron = 0; iNeuron < m_deltaE[ 0 ].size(); ++iNeuron )
      {
         // m_deltaE[ 0 ][ iNeuron ] *= m_dfdy[ 0 ][ iNeuron ];
      }


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

   composeGradient( gradient );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// propagateFwd
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mlp2::propagateForward( const RealVector& sourceLayer, RealVector& destLayer, const std::vector< RealVector >& weights )
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
void Mlp2::propagateBackward( const RealVector& sourceLayer, RealVector& destLayer, const std::vector< RealVector >& weights )
{
   assert( weights.size() == destLayer.size() );
   for ( size_t iDestNeuron = 0; iDestNeuron < destLayer.size(); ++iDestNeuron )
   {
      destLayer[ iDestNeuron ] = 0;
      assert( weights[ iDestNeuron ].size() == sourceLayer.size() );
      for ( size_t iSourceNeuron = 0; iSourceNeuron < sourceLayer.size(); ++iSourceNeuron )
      {
         destLayer[ iDestNeuron ] += sourceLayer[ iSourceNeuron ] * weights[ iDestNeuron ][ iSourceNeuron ];
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// applyActivationFunc
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mlp2::applyActivationFunc( const RealVector& neuronActivation, RealVector& neuronResponse )
{
   for ( size_t i = 0; i < neuronActivation.size(); ++i )
   {
      // neuronResponse[ i ] = tanh( neuronActivation[ i ] );
      neuronResponse[ i ] = 2 * neuronActivation[ i ];
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// calcDerivativesActivationFunc
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mlp2::calcDerivativesActivationFunc()
{
   for ( size_t iLayer = 0; iLayer < m_y.size(); ++iLayer )
   {
      for ( size_t iNeuron = 0; iNeuron < m_y[ iLayer ].size(); ++iNeuron )
      {
         double y = m_y[ iLayer ][ iNeuron ];
         // m_dfdy[ iLayer ][ iNeuron ] = 1 - y * y;
         m_dfdy[ iLayer ][ iNeuron ] = 2;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// composeGradient
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mlp2::composeGradient( RealVector& gradient )
{
   gradient.clear();

   for ( size_t iLayer = 0; iLayer < m_weightDerivatives.size(); ++iLayer )
   {
      for ( size_t iSource = 0; iSource < m_weightDerivatives[ iLayer ].size(); ++iSource )
      {
         for ( size_t iDest = 0; iDest < m_weightDerivatives[ iLayer ][ iSource ].size(); ++iDest )
         {
            gradient.push_back( m_weightDerivatives[ iLayer ][ iSource ][ iDest ] );
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// randomiseWeights
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mlp2::randomiseWeights( const Interval& interval )
{
   RandomNumberGenerator rng( 0 );
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
std::vector< double > Mlp2::getWeights()
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

void Mlp2::setWeights( const std::vector< double >& weights )
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

} /// namespace Mva
