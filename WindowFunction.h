#ifndef WINDOWFUNCTION_H
#define WINDOWFUNCTION_H

#include "RealVector.h"

namespace WaveAnalysis
{

/// Forward declare
class ShortTimeFftw;

/**
 * @brief Windowing functions for the Short Time Fourier Transform
 *
 * For formulas see the Wikipedia page "Window Function"
 */
class WindowFunction
{
   public:
      WindowFunction( size_t numSamples );
      virtual ~WindowFunction();

   public:
      size_t getSize() const;

   public:
      virtual double calc( size_t iSample ) const = 0;
      virtual WindowFunction* clone() const = 0;

   private:
      virtual void init() = 0;

   protected:
      mutable size_t m_numSamples;

};

class RectangularWindowFunction : public WindowFunction
{
   public:
      RectangularWindowFunction( size_t numSamples );

   public:
      double calc( size_t iSample ) const;
      WindowFunction* clone() const;

   private:
      void init();
};

class HannPoissonWindowFunction : public WindowFunction
{
   public:
      HannPoissonWindowFunction( size_t numSamples );

   public:
      double calc( size_t iSample ) const;
      WindowFunction* clone() const;

   private:
      void init();

   private:
      RealVector m_cachedValues;
};

class HanningWindowFunction : public WindowFunction
{
   public:
      HanningWindowFunction( size_t numSamples );

   public:
      double calc( size_t iSample ) const;
      WindowFunction* clone() const;

   private:
      void init();

   private:
      RealVector m_cachedValues;
};

class HanningDerivativeWindowFunction : public WindowFunction
{
   public:
      HanningDerivativeWindowFunction( size_t numSamples );

   public:
      double calc( size_t iSample ) const;
      WindowFunction* clone() const;

   private:
      void init();

   private:
      RealVector m_cachedValues;
};

class HanningTimeRampedWindowFunction : public WindowFunction
{
   public:
      HanningTimeRampedWindowFunction( size_t numSamples );

   public:
      double calc( size_t iSample ) const;
      WindowFunction* clone() const;

   private:
      void init();

   private:
      RealVector m_cachedValues;
};

} /// namespace WaveAnalysis

#endif // WINDOWFUNCTION_H
