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
      /**
       * Create a fit function with @param numParameters parameters.
       */
      FitFunctionBase( size_t numParameters );
      /**
       * Destructor
       */
      virtual ~FitFunctionBase();

   /**
    * Restating necessary overloads from IRealFunction (@see IRealFunction).
    */
   public:
      /**
       * Evaluate the function at point @param x (@see IRealFunction).
       */
      virtual double operator()( double x ) const = 0;
      /**
       * @see IRealFunction.
       */
      virtual IRealFunction* clone() const = 0;

   public:
      /**
       * Get the number of parameters.
       */
      size_t getNumParameters() const;
      /**
       * Set parameters.
       */
      void setParameters( const RealVector& parameters );
      /**
       * Get parameter @param parIndex.
       */
      double par( size_t parIndex ) const;

   private:
      RealVector     m_parameters;     //! The parameters of the fit function.
};

} /// namespace Math

#endif // FITFUNCTIONBASE_H
