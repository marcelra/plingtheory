#include "TwoTuple.h"

#include "RealVector.h"

#include <cassert>

namespace Math
{

TwoTuple::TwoTuple( size_t nElements ) :
   m_x( nElements ),
   m_y( nElements )
{}

TwoTuple::TwoTuple( const RealVector& xVec, const RealVector& yVec ) :
   m_x( xVec ),
   m_y( yVec )
{
   assert( xVec.size() == yVec.size() );
}

TwoTuple::TwoTuple( const TwoTuple& other ) :
   INTuple(),
   m_x( other.m_x ),
   m_y( other.m_y )
{
}

TwoTuple& TwoTuple::operator=( const TwoTuple& other )
{
   if ( this == &other )
   {
      return *this;
   }

   m_x = other.m_x;
   m_y = other.m_y;
   return *this;
}

INTuple::Ptr TwoTuple::clone() const
{
   return INTuple::Ptr( new TwoTuple( *this ) );
}

TwoTuple::~TwoTuple()
{}

size_t TwoTuple::getNumElements() const
{
   return m_x.size();
}

size_t TwoTuple::getNumVariables() const
{
   return 2;
}

const RealVector& TwoTuple::getVariable( size_t variableIndex ) const
{
   assert( variableIndex < 2 );
   return variableIndex == 0? m_x : m_y;
}

RealVector& TwoTuple::getVariable( size_t variableIndex )
{
   return const_cast< RealVector& >( const_cast< const TwoTuple& >( *this ).getVariable( variableIndex ) );
}

const RealVector& TwoTuple::getX() const
{
   return m_x;
}

const RealVector& TwoTuple::getY() const
{
   return m_y;
}

RealVector& TwoTuple::getX()
{
   return m_x;
}

RealVector& TwoTuple::getY()
{
   return m_y;
}

void TwoTuple::add( double x, double y )
{
   m_x.push_back( x );
   m_y.push_back( y );
}

void TwoTuple::addTuple( const RealVector& row )
{
   assert( row.size() == 2 );
   m_x.push_back( row[0] );
   m_y.push_back( row[1] );
}

} /// namespace Math
