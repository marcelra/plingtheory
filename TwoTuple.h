#ifndef TWOTUPLE_H
#define TWOTUPLE_H

#include "INTuple.h"

namespace Math
{

/**
 * @class TwoTuple
 * @brief Class that describes X vs Y data.
 * Implements a INTuple interface and data is stored vertically.
 */
class TwoTuple : public INTuple
{
   public:
      /**
       * Readability typedef.
       */
      typedef std::unique_ptr< TwoTuple > Ptr;

   public:
      /**
       * Construct an empty TwoTuple.
       */
      TwoTuple();
      /**
       * Construct an TwoTuple with @param nElements uninitialised (X,Y)-tuples.
       */
      TwoTuple( size_t nElements );
      /**
       * Construct an TwoTuple from two separate vectors: @param xVec and @param yVec.
       * It is required that both vector have the same lenght.
       */
      TwoTuple( const RealVector& xVec, const RealVector& yVec );

      /**
       * Copy-constructor.
       */
      TwoTuple( const TwoTuple& other );
      /**
       * Assigment operator.
       */
      TwoTuple& operator=( const TwoTuple& other );
      /**
       * Destructor.
       */
      virtual ~TwoTuple();

   /**
    * INTuple interface (@see INtuple).
    */
   public:
      size_t getNumElements() const;
      size_t getNumVariables() const;
      const RealVector& getVariable( size_t variableIndex ) const;
      RealVector& getVariable( size_t variableIndex );
      void addTuple( const RealVector& row );

   /**
    * Access X and Y arrays
    */
   public:
      const RealVector& getX() const;
      const RealVector& getY() const;

      RealVector& getX();
      RealVector& getY();

      double getX( size_t iElement ) const;
      double getY( size_t iElement ) const;

   /**
    * Extend with a (x,y)-tuple.
    */
   public:
      void add( double x, double y );

   private:
      RealVector        m_x;        //! x-data
      RealVector        m_y;        //! y-data
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods TwoTuple
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getX
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TwoTuple::getX( size_t iElement ) const
{
   return m_x[ iElement ];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TwoTuple::getY( size_t iElement ) const
{
   return m_y[ iElement ];
}


} /// namespace Math

#endif // TWOTUPLE_H
