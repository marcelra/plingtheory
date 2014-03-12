#ifndef WEIGHTFUNCTION_H
#define WEIGHTFUNCTION_H

namespace Math
{

/// TODO: doc
class IWeightFunction
{
   public:
      virtual ~IWeightFunction();
      virtual double getMin() const = 0;
      virtual double getMax() const = 0;
      virtual double getWeight( double x ) const = 0;
};

/// TODO: doc
class GaussianWeightFunction
{
   public:
      GaussianWeightFunction( double min, double max, double sigma );
      ~GaussianWeightFunction();

      double getMin() const;
      double getMax() const;
      double getWeight( double x ) const;

   private:
      double      m_min;
      double      m_max;
      double      m_argFac;
      double      m_norm;
};

} /// namespace Math

#endif // WEIGHTFUNCTION_H
