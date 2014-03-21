#ifndef NEWTONSOLVER1D_H
#define NEWTONSOLVER1D_H

#include "LoggerClient.h"
#include "IRealFuncWithDerivative.h"

namespace Math
{

class NewtonSolver1D : public LoggerClient
{
   public:
      class Result
      {
         public:
            enum Status
            {
               Converged = 0,
               NotConverged,
               FailedStationary
            };
         public:
            Result();
         public:
            double getSolution() const;
            double getEpsilon() const;
            bool isConverged() const;
            Status getStatus() const;

         private:
            friend class NewtonSolver1D;
            Status m_status;
            double m_eps;
            double m_solution;

      };

   public:
      NewtonSolver1D( const IRealFuncWithDerivative& function, double value = 0 );

      void setTolerance( double tolerance );

      Result solve( double startValue, size_t nIterationsMax );

   private:
      const IRealFuncWithDerivative&      m_func;
      double                              m_tolerance;
      double                              m_value;
      static double                       s_failDerivThreshold;
};

} /// namespace Math

#endif // NEWTONSOLVER1D_H
