#ifndef REGULAR2DHISTOGRAM_H
#define REGULAR2DHISTOGRAM_H

#include <cstddef>
#include <vector>

namespace Math
{

/**
 * @class Regular2DHistogram
 * @brief 2D histogram with equal sized bins.
 */
class Regular2DHistogram
{
   public:
      /**
       * Create 2D histogram with @param nBinsX bins in the x direction and @param nBinsY bins in the y direction.
       */
      Regular2DHistogram( size_t nBinsX, double xMin, double xMax, size_t nBinsY, double yMin, double yMax );

      /**
       * Get the number of bins in x direction.
       */
      size_t getNumBinsX() const;
      /**
       * Get the number of bins in y direction.
       */
      size_t getNumBinsY() const;

      /**
       * Get the bin width in the x direction.
       */
      double getBinWidthX() const;
      /**
       * Get the bin width in the y direction.
       */
      double getBinWidthY() const;

      /**
       * Set the bin content for bin @param iBinX and @param iBinY to value.
       * Previous value will be overwritten.
       */
      void setBinContent( size_t iBinX, size_t iBinY, double value );
      /**
       * Get the bin contents for bin @param iBinX and @param iBinY.
       */
      double getBinContent( size_t iBinX, size_t iBinY ) const;

      /**
       * Add new entry in histogram with weight @param weight, located at (@param x, @param y).
       */
      void add( double x, double y, double weight = 1.0 );

   private:
      /**
       * Retrieve bin index in x direction for value @param x.
       */
      size_t getBinIndexX( double x ) const;
      /**
       * Retrieve bin index in y direction for value @param y.
       */
      size_t getBinIndexY( double y ) const;

   private:
      std::vector< std::vector< double > >      m_contents;       //! Bin contents. First index is x, second index is y.
      double                                    m_missedContents; //! Values that have been add using the method add that do not fall inside a bin.

      double  m_xMin;         //! Mininum along the x direction.
      double  m_xMax;         //! Maximum along the x direction.
      double  m_xBinWidth;    //! Bin width along the x direction.
      size_t  m_nBinsX;       //! Number of bins along the x direction.
      double  m_yMin;         //! Mininum along the y direction.
      double  m_yMax;         //! Maximum along the y direction.
      double  m_yBinWidth;    //! Bin width along the y direction.
      size_t  m_nBinsY;       //! Number of bins along the y direction.
};

} /// namespace Math

#endif // REGULAR2DHISTOGRAM_H
