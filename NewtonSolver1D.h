#ifndef NEWTONSOLVER1D_H
#define NEWTONSOLVER1D_H

#include "IRealFuncWithDerivative.h"

namespace Math
{

class NewtonSolver1D
{
   public:
      class Result
      {
         public:
            void setSolution( double solution );
            double getSolution() const;
         private:
           double m_eps;
           double m_solution;
      };

   public:
      NewtonSolver1D( const IRealFuncWithDerivative& function, double value = 0 );

      Result solve( double startValue, size_t nIterationsMax );

   private:
      const IRealFuncWithDerivative&      m_func;
      double                              m_tolerance;
      double                              m_value;
};

} /// namespace Math

#endif // NEWTONSOLVER1D_H
