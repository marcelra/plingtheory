#ifndef CHI2FITOBJECTIVE_H
#define CHI2FITOBJECTIVE_H

#include "IObjectiveFunction.h"
#include "FitFunctionBase.h"

namespace Math
{

/**
 * @class Chi2FitObjective
 * @brief Implements an IObjectiveFunction interface for calculating the chi2 of a data set.
 */
class Chi2FitObjective : public IObjectiveFunction
{
   public:
      /**
       * Construct a Chi2FitObjective object. The data set is specified by @param xData, @param yData. The variance squared
       * for y is given by @param ySigma2. The fit function is @param function and should be a valid pointer.
       *
       * Notes:
       * - Note that @param function will be modified if evaluate is called.
       * - The data is not owned by this class.
       * - The instance cannot be shared among threads since it modifies @param function.
       */
      Chi2FitObjective( const RealVector& xData, const RealVector& yData, const RealVector& ySigma2, FitFunctionBase* function );

      /**
       * Evaluate chi2 as function of the parameters of the fit function given in the constructor.
       */
      double evaluate( const RealVector& x ) const;

      /**
       * Get the number of parameters of the fit function.
       */
      size_t getNumParameters() const;

   private:
      const RealVector&        m_xData;         //! x values of data set.
      const RealVector&        m_yData;         //! y values of data set.
      const RealVector&        m_ySigma2;       //! y variance squared of data set.
      mutable FitFunctionBase*    m_func;          //! function to be fitted.
};

} /// namespace Math

#endif // CHI2FITOBJECTIVE_H
