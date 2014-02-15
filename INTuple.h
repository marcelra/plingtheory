#ifndef INTUPLE_H
#define INTUPLE_H

#include "RealVector.h"

#include "WeightFunction.h"

namespace Math
{

class INTuple
{
   public:
      typedef std::auto_ptr< INTuple > Ptr;

   public:
      virtual INTuple::Ptr clone() const = 0;
      virtual ~INTuple();

   public:
      virtual size_t getNumElements() const = 0;
      virtual size_t getNumVariables() const = 0;

   public:
      virtual const RealVector& getVariable( size_t variableIndex ) const = 0;
      virtual RealVector& getVariable( size_t variableIndex ) = 0;

      virtual RealVector getRow( size_t rowIndex ) const;
      virtual void setRow( size_t rowIndex, const RealVector& rowValues );

   public:
      virtual INTuple::Ptr sortVariable( size_t variableIndex ) const;
};


} /// namespace Math


#endif // INTUPLE_H
