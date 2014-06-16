#include "MlpTrainer.h"

#include "MultiLayerPerceptron.h"

namespace Mva
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MlpTrainer::MlpTrainer( MultiLayerPerceptron& mlp, const std::string& algName, const AlgorithmBase* parent ) :
   AlgorithmBase( algName, parent ),
   m_mlp( mlp ),
   m_trainDataInput( 0 ),
   m_trainDataOutput( 0 ),
   m_testDataInput( 0 ),
   m_testDataOutput( 0 ),
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
/// setTestData
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MlpTrainer::setTestData( const std::vector< RealVector >& inputData, const std::vector< RealVector >& outputData )
{
   m_testDataInput = &inputData;
   m_testDataOutput = &outputData;
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// calculateErrorTestSet
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double MlpTrainer::calculateErrorTestSet()
{
   if ( !m_testDataInput )
   {
      return 0;
   }
   double error = 0;
   for ( size_t i = 0; i < m_testDataInput->size(); ++i )
   {
      error += m_mlp.calcError( (*m_testDataInput)[ i ], (*m_testDataOutput)[ i ] );
   }
   error /= m_testDataInput->size();
   return error;
}


} /// namespace Mva
