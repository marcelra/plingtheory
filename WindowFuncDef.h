#ifndef WINDOWFUNCDEF_H
#define WINDOWFUNCDEF_H

#include "WindowFunction.h"

namespace WaveAnalysis
{

/// TODO: doc
class WindowFuncDef
{
   public:
      virtual ~WindowFuncDef();
      virtual WindowFunction* createWindowFunction( size_t numSamples ) const = 0;
      virtual WindowFuncDef* clone() const = 0;
      virtual bool isInvertible() const;
};

inline bool WindowFuncDef::isInvertible() const
{
   return true;
}

class RectangularWindowFuncDef : public WindowFuncDef
{
   public:
      WindowFunction* createWindowFunction( size_t numSamples ) const;
      WindowFuncDef* clone() const;
};

class HannPoissonWindowFuncDef : public WindowFuncDef
{
   public:
      WindowFunction* createWindowFunction( size_t numSamples ) const;
      WindowFuncDef* clone() const;
};

class HanningWindowFuncDef : public WindowFuncDef
{
   public:
      WindowFunction* createWindowFunction( size_t numSamples ) const;
      WindowFuncDef* clone() const;
};

class HanningDerivativeWindowFuncDef : public WindowFuncDef
{
   public:
      WindowFunction* createWindowFunction( size_t numSamples ) const;
      WindowFuncDef* clone() const;
      bool isInvertible() const { return false; }
};

class HanningTimeRampedWindowFuncDef : public WindowFuncDef
{
   public:
      WindowFunction* createWindowFunction( size_t numSamples ) const;
      WindowFuncDef* clone() const;
      bool isInvertible() const { return false; }
};

} /// namespace WaveAnalysis

#endif // WINDOWFUNCDEF_H
