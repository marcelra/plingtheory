#ifndef FITFUNCTIONBASE_H
#define FITFUNCTIONBASE_H

#include "IRealFunction.h"

namespace Math
{

/**
 * @class FitFunctionBase
 * @brief Base class for a parametric functions that can be used for fitting.
 */
class FitFunctionBase : public IRealFunction
{
   public:
      FitFunctionBase( size_t numParameters = 2 );
      virtual ~FitFunctionBase();

      size_t getNumParameters() const;
      void setParameters( const RealVector& parameters );

      double operator()( double x ) const;
      IRealFunction* clone() const;

   protected:
      double par( size_t parIndex ) const;

   private:
      RealVector     m_parameters;
};

} /// namespace Math

#endif // FITFUNCTIONBASE_H
