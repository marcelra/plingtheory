#ifndef IREALFUNCTION_H
#define IREALFUNCTION_H

#include <memory>

#include "RealVector.h"

namespace Math
{

class IRealFunction
{
   public:
      typedef std::auto_ptr< IRealFunction > Ptr;

      virtual ~IRealFunction();

      virtual double operator()( double x ) const = 0;
      virtual IRealFunction* clone() const = 0;

      RealVector evalMany( const RealVector& argVec ) const;
};

} /// namespace Math

#endif // IREALFUNCTION_H
