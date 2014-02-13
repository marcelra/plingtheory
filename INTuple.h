#ifndef INTUPLE_H
#define INTUPLE_H

#include "RealVector.h"

namespace Math
{

class INTuple
{
   public:
      virtual ~INTuple();

      virtual size_t getNumElements() const = 0;
      virtual size_t getNumVariables() const = 0;

      virtual const RealVector& getVariable( size_t variableIndex ) const = 0;
      virtual RealVector& getVariable( size_t variableIndex ) = 0;

      virtual RealVector getRow( size_t rowIndex ) const;
      virtual void setRow( size_t rowIndex, const RealVector& rowValues );

      virtual void sortVariable( size_t variableIndex );
};


} /// namespace Math


#endif // INTUPLE_H
