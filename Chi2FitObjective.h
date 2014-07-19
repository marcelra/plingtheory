#ifndef CHI2FITOBJECTIVE_H
#define CHI2FITOBJECTIVE_H

#include "IObjectiveFunction.h"
#include "IRealFunction.h"

namespace Math
{

class IFitFunction : public IRealFunction
{
   public:
      IFitFunction( size_t numParameters = 2 );
      virtual ~IFitFunction();

      size_t getNumParameters() const;
      void setParameters( const RealVector& parameters );

      double operator()( double x ) const;
      IRealFunction* clone() const;

   protected:
      double par( size_t parIndex ) const;

   private:
      RealVector     m_parameters;
};


class Chi2FitObjective : public IObjectiveFunction
{
   public:
      Chi2FitObjective( const RealVector& xData, const RealVector& yData, const RealVector& ySigma2, IFitFunction* function );

      double evaluate( const RealVector& x ) const;
      size_t getNumParameters() const;

   private:
      const RealVector&        m_xData;
      const RealVector&        m_yData;
      const RealVector&        m_ySigma2;
      mutable IFitFunction*    m_func;
};

} /// namespace Math

#endif // CHI2FITOBJECTIVE_H
