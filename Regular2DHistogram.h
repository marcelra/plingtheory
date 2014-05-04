#ifndef REGULAR2DHISTOGRAM_H
#define REGULAR2DHISTOGRAM_H

#include <cstddef>
#include <vector>

namespace Math
{

class Regular2DHistogram
{
   public:
      Regular2DHistogram( size_t nBinsX, double xMin, double xMax, size_t nBinsY, double yMin, double yMax );

      size_t getNumBinsX() const;
      size_t getNumBinsY() const;

      double getBinWidthX() const;
      double getBinWidthY() const;

      void setBinContent( size_t iBinX, size_t iBinY, double value );
      double getBinContent( size_t iBinX, size_t iBinY ) const;

      void add( double x, double y, double weight = 1.0 );

   private:
      size_t getBinIndexX( double x ) const;
      size_t getBinIndexY( double y ) const;

   private:
      std::vector< std::vector< double > >      m_contents;
      double                                    m_missedContents;

      double  m_xMin;
      double  m_xMax;
      double  m_xBinWidth;
      size_t  m_nBinsX;
      double  m_yMin;
      double  m_yMax;
      double  m_yBinWidth;
      size_t  m_nBinsY;
};

} /// namespace Math

#endif // REGULAR2DHISTOGRAM_H
