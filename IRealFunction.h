#ifndef IREALFUNCTION_H
#define IREALFUNCTION_H

#include <memory>

#include "RealVector.h"

namespace Math
{

/**
 * @class IRealFunction
 * @brief Interface for real to real functions.
 */
class IRealFunction
{
   public:
      /**
       * Readability typedef of unique_ptr.
       */
      typedef std::unique_ptr< IRealFunction > Ptr;

   public:
      /**
       * Virtual destructor.
       */
      virtual ~IRealFunction();

      /**
       * Evaluate function at point @param x.
       */
      virtual double operator()( double x ) const = 0;
      /**
       * Clone the function.
       */
      virtual IRealFunction* clone() const = 0;

      /**
       * Eval function at points in @param argVec.
       */
      RealVector evalMany( const RealVector& argVec ) const;
};

} /// namespace Math

#endif // IREALFUNCTION_H
