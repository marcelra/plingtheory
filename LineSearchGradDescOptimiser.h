#ifndef LINESEARCHGRADDESCOPTIMISER_H
#define LINESEARCHGRADDESCOPTIMISER_H

#include "AlgorithmBase.h"
#include "IObjectiveFunction.h"

namespace Math
{

/**
 * @class LineSearchGradDescOptimiser
 * @brief A gradient descent optimiser using backtracking line search.
 */
class LineSearchGradDescOptimiser : public AlgorithmBase
{
   public:
      /**
       * Minimise @param objFunc (@see AlgorithmBase).
       */
      LineSearchGradDescOptimiser( const IObjectiveFunction& objFunc, const std::string& algorithmName = "LineSearchGradDescOptimiser", const AlgorithmBase* parent = 0 );

      /**
       * Solve the problem.
       */
      RealVector solve( RealVector startValue ) const;

   public:
      /**
       * Set the maximum number of iterations.
       */
      void setMaxIterations( size_t maxIterations );
      /**
       * Sets the convergence criterion: minimum length of the gradient.
       */
      void setMinGradLength( double minGradLength );
      /**
       * Set the convergence criterion: minimum objective function value change between two iterations.
       */
      void setMinObjFuncValChange( double minObjFuncValChange );

   public:
      /**
       * Sets the first constant of the Wolfe conditions used in the backtracking line search algorithm.
       */
      void setWolfeConstant1( double c1 );
      /**
       * Sets the contraction factor in the backtracking line search algorithm.
       */
      void setContractionFactor( double rho );

   private:
      const IObjectiveFunction&     m_objFunc;              //! The objective function to be minimised.
      size_t                        m_maxIterations;        //! The maximum number of iterations.
      double                        m_minGradModulus;       //! The minimum gradient modulus.
      double                        m_minObjFuncValChange;  //! The miniumum objective function value change.
      double                        m_c1;                   //! The first Wolfe condition constant.
      double                        m_rho;                  //! The backtracking line search contraction factor.

};

} /// namespace Math

#endif // LINESEARCHGRADDESCOPTIMISER_H
