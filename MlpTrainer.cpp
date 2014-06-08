#include "MlpTrainer.h"

namespace Mva
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MlpTrainer::MlpTrainer( Mlp2& mlp, const std::string& algName, const AlgorithmBase* parent ) :
   AlgorithmBase( algName, parent ),
   m_mlp( mlp ),
   m_error( std::numeric_limits< double >::max() ),
   m_numIterations( 100 ),
   m_errorTolerance( 0 )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MlpTrainer::~MlpTrainer()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setInputData
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MlpTrainer::setInputData( const std::vector<RealVector>& inputData, const std::vector<RealVector>& outputData )
{
   m_trainDataInput = &inputData;
   m_trainDataOutput = &outputData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setNumIterations
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MlpTrainer::setNumIterations( size_t nIter )
{
   m_numIterations = nIter;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setErrorTolerance
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MlpTrainer::setErrorTolerance( double errorTolerance )
{
   m_errorTolerance = errorTolerance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// isConverged
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MlpTrainer::isConverged() const
{
   if ( m_error < m_errorTolerance )
   {
      return true;
   }
   return false;
}

} /// namespace Mva
