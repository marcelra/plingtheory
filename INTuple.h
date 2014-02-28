#ifndef INTUPLE_H
#define INTUPLE_H

#include "RealVector.h"

#include "WeightFunction.h"

namespace Math
{

/**
 * @class INTuple
 * @brief Interface class for data set consisting of tuples of continuous (real) values.
 *
 * Internally the data is stored per variable which means that `vertical' queries (i.e. @see getVariable) are fast, but
 * `horizontal' queries (i.e. @see getTuple ) are slow. (This requirement is implicit from the getVariable signature).
 */
class INTuple
{
   public:
      /**
       * Readability typedef
       */
      typedef std::auto_ptr< INTuple > Ptr;

   public:
      /**
       * Destructor
       */
      virtual ~INTuple();

   public:
      /**
       * Get the number of tuples in the data set.
       */
      virtual size_t getNumElements() const = 0;
      /**
       * Get the dimensionality of the tuple.
       */
      virtual size_t getNumVariables() const = 0;

   public:
      /**
       * Get all the data for one variable.
       */
      virtual const RealVector& getVariable( size_t variableIndex ) const = 0;
      virtual RealVector& getVariable( size_t variableIndex ) = 0;

      /**
       * Get a tuple (row).
       */
      virtual RealVector getTuple( size_t rowIndex ) const;
      /**
       * Set a tuple (row).
       */
      virtual void setTuple( size_t rowIndex, const RealVector& rowValues );
      /**
       * Add a tuple (row).
       */
      virtual void addTuple( const RealVector& values ) = 0;
};

} /// namespace Math

#endif // INTUPLE_H
