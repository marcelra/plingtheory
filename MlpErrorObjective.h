#ifndef MLPERROROBJECTIVE_H
#define MLPERROROBJECTIVE_H

#include "IObjectiveFunction.h"
#include "MultiLayerPerceptron.h"

namespace Mva
{

/**
 * @class MlpErrorObjective
 * @brief Square difference between target and MLP network evaluation as function of weights. Note that the target and
 * evaluation point have to be specified at construction and are no parameters during evaluation.
 */
class MlpErrorObjective : public Math::IObjectiveFunction
{
   public:
      /**
       * Constructor. @see @brief.
       */
      MlpErrorObjective( MultiLayerPerceptron& mlp, RealVector& eval, RealVector& target );

      /**
       * Evaluate the error as function of @param weights.
       */
      double evaluate( const RealVector& weights ) const;

      /**
       * Get the number of weights.
       */
      size_t getNumParameters() const;

   private:
      MultiLayerPerceptron&   m_mlp;			//! MultiLayerPerceptron that is doing the evaluations.
      RealVector			      m_eval;			//! Evaluation point in input space.
      RealVector			      m_target;		//! Target output.
};

} /// namespace Mva

#endif // MLPERROROBJECTIVE_H
