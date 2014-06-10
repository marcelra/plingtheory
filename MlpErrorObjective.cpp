#include "MlpErrorObjective.h"

namespace Mva
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MlpErrorObjective::MlpErrorObjective( MultiLayerPerceptron& mlp, RealVector& eval, RealVector& target ) :
   m_mlp( mlp ),
   m_eval( eval ),
   m_target( target )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// evaluate
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double MlpErrorObjective::evaluate( const RealVector& weights ) const
{
   double error;
   m_mlp.setWeights( weights );
   RealVector&& grad = m_mlp.calcErrorAndGradient( m_eval, m_target, error );

   /// Suppress compiler error.
   assert( &grad );
   return error;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getNumParameters
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t MlpErrorObjective::getNumParameters() const
{
   return m_mlp.getWeights().size();
}

} /// namespace Mva