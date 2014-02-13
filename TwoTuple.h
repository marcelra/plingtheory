#ifndef TWOTUPLE_H
#define TWOTUPLE_H

#include "INTuple.h"

namespace Math
{

class TwoTuple : public INTuple
{
   public:
      TwoTuple( size_t nElements );
      TwoTuple( const RealVector& xVec, const RealVector& yVec );

      TwoTuple( const TwoTuple& other );
      TwoTuple& operator=( const TwoTuple& other );

      virtual ~TwoTuple();

      size_t getNumElements() const;
      size_t getNumVariables() const;
      const RealVector& getVariable( size_t variableIndex ) const;
      RealVector& getVariable( size_t variableIndex );

      const RealVector& getX() const;
      const RealVector& getY() const;

      RealVector& getX();
      RealVector& getY();

   private:
      RealVector        m_x;
      RealVector        m_y;
};

} /// namespace Math

#endif // TWOTUPLE_H
