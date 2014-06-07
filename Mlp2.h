#ifndef MLP2_H
#define MLP2_H

#include <RealVector.h>
#include "Interval.h"

namespace Mva
{

class Mlp2
{
   public:
      Mlp2( size_t numInputNodes, size_t numOutputNodes, const std::vector< size_t >& hiddenLayerStructure );

      RealVector evaluate( const RealVector& x );

      void calcErrorAndGradient( const RealVector& input, const RealVector& target, double& error, RealVector& gradient );
      std::vector< double > getWeights();
      void setWeights( const std::vector< double >& weights );

      void propagateForward( const RealVector& sourceLayer, RealVector& destLayer, const std::vector< RealVector >& weights );
      void propagateBackward( const RealVector& sourceLayer, RealVector& destLayer, const std::vector< RealVector >& weights );

      void applyActivationFunc( const RealVector& neuronActivation, RealVector& neuronResponse );
      void calcDerivativesActivationFunc();

      void randomiseWeights( const Interval& interval );

      void composeGradient( RealVector& gradient );

   /// TODO: public for development
   public:
      std::vector< std::vector< RealVector > > m_weights;

   private:
      RealVector						              m_input;
      RealVector						              m_output;
      std::vector< RealVector >	              m_y;
      std::vector< RealVector >	              m_x;

      std::vector< RealVector >					  m_deltaE;
      RealVector              					  m_deltaEInput;
      RealVector              					  m_deltaEOutput;
      std::vector< RealVector >					  m_dfdy;

      /// TODO: not used yet
      std::vector< RealVector >					  m_inputScale;
      std::vector< RealVector >					  m_inputOffset;
      std::vector< RealVector >					  m_outputScale;
      std::vector< RealVector >					  m_outputOffset;

      std::vector< std::vector< RealVector > > m_weightDerivatives;
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
