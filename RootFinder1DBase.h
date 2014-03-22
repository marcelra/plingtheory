#ifndef ROOTFINDER1DBASE_H
#define ROOTFINDER1DBASE_H

#include <cstddef>

namespace Math
{

class RootFinder1DBase
{
   public:
      class Result
      {
         public:
            enum Status
            {
               Converged = 0,
               NotConverged,
               Failed
            };

         public:
            Result();

         public:
            double getSolution() const;
            double getEpsilon() const;
            bool isConverged() const;
            Status getStatus() const;

         protected:

            friend class NewtonSolver1D;
            friend class BisectionSolver1D;

            Status m_status;
            double m_eps;
            double m_solution;
      };

   public:
      RootFinder1DBase();
      virtual ~RootFinder1DBase();

      void setTolerance( double tolerance );
      void setMaxIterations( size_t maxIterations );

   protected:
      double                              m_solutionTolerance;
      size_t                              m_maxIterations;
};

} /// namespace Math

#endif // IROOTFINDER1D_H
