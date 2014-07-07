#ifndef PREDEFINEDREALFUNCTIONS_H
#define PREDEFINEDREALFUNCTIONS_H

#include "IRealFunction.h"

namespace Math
{

class Log10Function : public IRealFunction
{
   public:
      Log10Function( double min = 1e-12 );

      double operator()( double x ) const;
      IRealFunction* clone() const;

   private:
      double      m_min;
};

} /// namespace Math

#endif // PREDEFINEDREALFUNCTIONS_H
