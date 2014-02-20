#ifndef ACCUMARRAY_H
#define ACCUMARRAY_H

#include "RealVector.h"
#include "TwoTuple.h"

namespace Math
{

class IAccumArray
{
   public:
      class Bin
      {
         public:
            Bin( double minX, double maxX );
            ~Bin();
            Bin( const Bin& other );
            Bin& operator=( const Bin& other );

         public:
            double getMinX() const;
            double getMaxX() const;
            double getContents() const;

            void add( double x, double value );

            const TwoTuple& getEntries() const;
            TwoTuple& getEntries();

         private:
            double         m_contents;
            TwoTuple       m_elements;
            double         m_minX;
            double         m_maxX;
      };

   public:
      virtual ~IAccumArray();

   public:
      virtual double getBinContent( size_t iBin ) const = 0;
      virtual double getMissedContent() const = 0;
      virtual double getUnderflow() const = 0;
      virtual double getOverflow() const = 0;

      virtual const TwoTuple& getBinEntries( size_t iBin ) const = 0;
      virtual const std::vector< Bin >& getBins() const = 0;

      virtual size_t getNumBins() const = 0;
      virtual double getMinX() const = 0;
      virtual double getMaxX() const = 0;

      virtual size_t findBin( double x ) const = 0;
      virtual Bin getBin( size_t binIndex ) const = 0;
      virtual void add( double x, double value ) = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods IAccumArray::Bin
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double IAccumArray::Bin::getMinX() const
{
   return m_minX;
}

inline double IAccumArray::Bin::getMaxX() const
{
   return m_maxX;
}

inline double IAccumArray::Bin::getContents() const
{
   return m_contents;
}

inline void IAccumArray::Bin::add( double x, double value )
{
   if ( x >= m_minX && x < m_maxX )
   {
      m_contents += value;
      m_elements.add( x, value );
   }
}

inline const TwoTuple& IAccumArray::Bin::getEntries() const
{
   return m_elements;
}

inline TwoTuple& IAccumArray::Bin::getEntries()
{
   return m_elements;
}

} /// namespace Math

#endif // ACCUMARRAY_H
