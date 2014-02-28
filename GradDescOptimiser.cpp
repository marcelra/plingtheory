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
GradDescOptimiser::GradDescOptimiser( const IObjectiveFunction& func, const RealVector& startValues ) :
   m_func( func ),
   m_input( startValues ),
   m_maxIterations( 20 ),
   m_objValChangeConvergence( 1e-6 ),
   m_useLineSearch( true )
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

   Logger msg( "GradDescOptimiser" );
   msg << Msg::Info << "Optimising Objective function with " << m_func.getNumParameters() << " parameters..." << Msg::EndReq;

   size_t iter = 0;
   double gamma = 1e-6;

   RealVector bestParameter = m_input;
   double bestValue = currentObjective;
   double gradLength;

   msg << Msg::Debug << "Starting iterations" << Msg::EndReq;
   do
   {
      // double oldObjective = currentObjective;
      const RealVector& grad = m_func.calculateGradient( m_input );
      if ( m_useLineSearch )
      {
         LineSearchObjective lineSearchObj( m_func, m_input, grad );
         RealVector gammaVec( 1, gamma );
         GradDescOptimiser lineSearch( lineSearchObj, gammaVec );

         /// We do not want infinite recursion
         lineSearch.setUseLineSearch( false );
         lineSearch.setConvergenceCriterion( 1e-6 );
         lineSearch.setMaxIterations( 10 );

         gamma = fabs( lineSearch.solve()[0] );
      }

      m_input = m_input - gamma*grad;

      currentObjective = m_func.evaluate( m_input );
      // solutionChange = oldObjective - currentObjective;
      gradLength = grad*grad;

      msg << Msg::Verbose << "Objective = " << currentObjective << Msg::EndReq;
      msg << Msg::Verbose << "Grad = " << grad << Msg::EndReq;
      ++iter;

      if ( currentObjective < bestValue )
      {
         bestParameter = m_input;
         bestValue = currentObjective;
      }
   }
   while ( ( gradLength > m_objValChangeConvergence ) && iter < m_maxIterations );

   msg << Msg::Debug << "Num iterations needed: " << iter << Msg::EndReq;
   msg << Msg::Debug << "Solution = " << m_input << Msg::EndReq;

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
/// setUseLineSearch
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GradDescOptimiser::setUseLineSearch( bool useLineSearch )
{
   m_useLineSearch = useLineSearch;
}

} /// namespace Math
