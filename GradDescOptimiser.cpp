#include "GradDescOptimiser.h"

#include "IObjectiveFunction.h"
#include "LineSearchObjective.h"
#include "Logger.h"

#include <cmath>

namespace Math
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GradDescOptimiser::GradDescOptimiser( const IObjectiveFunction& func, const RealVector& startValues, const std::string& algorithmName, const AlgorithmBase* parent ) :
   AlgorithmBase( algorithmName, parent ),
   m_func( func ),
   m_gamma( 0.1 ),
   m_input( startValues ),
   m_maxIterations( 20 ),
   m_objValChangeConvergence( 1e-6 )
{
   assert( m_func.getNumParameters() > 0 && m_func.getNumParameters() == startValues.size() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GradDescOptimiser::~GradDescOptimiser()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// solve
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVector GradDescOptimiser::solve()
{
   double currentObjective = m_func.evaluate( m_input );

   getLogger() << Msg::Debug << "Optimising Objective function with " << m_func.getNumParameters() << " parameters..." << Msg::EndReq;

   size_t iter = 0;
   double gamma = m_gamma;

   RealVector bestParameter = m_input;
   double bestValue = currentObjective;
   double gradLength;

   getLogger() << Msg::Debug << "Starting iterations" << Msg::EndReq;
   do
   {
      // double oldObjective = currentObjective;
      const RealVector& grad = m_func.calculateGradient( m_input );

      m_input = m_input - gamma*grad;

      currentObjective = m_func.evaluate( m_input );
      // solutionChange = oldObjective - currentObjective;
      gradLength = grad*grad;

      getLogger() << Msg::Verbose << "Objective = " << currentObjective << Msg::EndReq;
      getLogger() << Msg::Verbose << "Grad = " << grad << Msg::EndReq;
      ++iter;

      if ( currentObjective < bestValue )
      {
         bestParameter = m_input;
         bestValue = currentObjective;
      }
   }
   while ( ( gradLength > m_objValChangeConvergence ) && iter < m_maxIterations );

   getLogger() << Msg::Debug << "Num iterations needed: " << iter << Msg::EndReq;
   getLogger() << Msg::Debug << "Solution = " << m_input << Msg::EndReq;

   return m_input;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setMaxIterations
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GradDescOptimiser::setMaxIterations( size_t maxIterations )
{
   m_maxIterations = maxIterations;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setConvergenceCriterion
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GradDescOptimiser::setConvergenceCriterion( double objValChange )
{
   m_objValChangeConvergence = objValChange;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setGamma
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GradDescOptimiser::setGamma( double gamma )
{
   m_gamma = gamma;
}

} /// namespace Math
