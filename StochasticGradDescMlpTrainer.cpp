#include "StochasticGradDescMlpTrainer.h"

#include "Logger.h"
#include "MultiLayerPerceptron.h"
#include "Utils.h"

namespace Mva
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StochasticGradDescMlpTrainer::StochasticGradDescMlpTrainer( MultiLayerPerceptron& mlp, const std::string& algName, const AlgorithmBase* parent ) :
   MlpTrainer( mlp, algName, parent ),
   m_eta( 0.01 ),
   m_batchSize( 25 ),
   m_numIterationsPerEpoch( 10 ),
   m_rng( 64293 )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setEta
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void StochasticGradDescMlpTrainer::setEta( double eta )
{
   m_eta = eta;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setBatchSize
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void StochasticGradDescMlpTrainer::setBatchSize( size_t batchSize, size_t numIterationsPerEpoch )
{
   m_batchSize = batchSize;
   m_numIterationsPerEpoch = numIterationsPerEpoch;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// makeBatch
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector< size_t > StochasticGradDescMlpTrainer::makeBatch()
{
   std::vector< size_t > result;
   if ( m_batchSize == 0 )
   {
      result = Utils::createRange( 0, m_trainDataInput->size() );
   }
   else
   {
      result.resize( m_batchSize );
      for ( size_t i = 0; i < m_batchSize; ++i )
      {
         size_t proposedIndex = 0;
         bool accepted = false;

         for ( size_t iTry = 0; iTry < s_numTriesAddSampleToBatch; ++iTry )
         {
            proposedIndex = m_rng.uniform( 0, m_trainDataInput->size() );
            double acceptDraw = m_rng.uniform( 0, m_error );
            if ( acceptDraw < m_sampleErrors[ proposedIndex ] )
            {
               accepted = true;
               break;
            }
         }

         ++m_numSampleProposals;
         if ( !accepted )
         {
            ++m_numFailedSampleProposals;
         }

         result[ i ] = proposedIndex;

      }
   }
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// calculateInitialSampleErrors
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double StochasticGradDescMlpTrainer::calculateErrorsFullSet()
{
   double totalError = 0;

   for ( size_t iSample = 0; iSample < m_trainDataInput->size(); ++iSample )
   {
      double sampleError = m_mlp.calcError( (*m_trainDataInput)[ iSample ], (*m_trainDataOutput)[ iSample ] );
      updateSampleError( iSample, sampleError );
      totalError += sampleError;
   }
   return totalError / m_trainDataInput->size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// train
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void StochasticGradDescMlpTrainer::train()
{
   assert( m_trainDataInput );

   m_numSampleProposals = 0;
   m_numFailedSampleProposals = 0;

   RealVector&& weights = m_mlp.getWeights();

   if ( m_sampleErrors.size() != m_trainDataInput->size() )
   {
      m_sampleErrors.resize( m_trainDataInput->size() );
   }

   calculateErrorsFullSet();

   for ( size_t iIter = 0; iIter < m_numIterations; ++iIter )
   {
      for ( size_t iSubIter = 0; iSubIter < m_numIterationsPerEpoch; ++iSubIter )
      {
         RealVector gradient( weights.size(), 0 );
         std::vector< size_t >&& batch = makeBatch();

         for ( size_t iSample = 0; iSample < batch.size(); ++iSample )
         {
            size_t sampleIndex = batch[ iSample ];
            double sampleError;
            RealVector sampleGradient = m_mlp.calcErrorAndGradient( (*m_trainDataInput)[ sampleIndex ], (*m_trainDataOutput)[ sampleIndex ], sampleError );
            gradient = gradient + sampleGradient;
            updateSampleError( sampleIndex, sampleError );
         }

         gradient = gradient * ( 1.0 / batch.size() );

         weights = weights - gradient * m_eta;
         m_mlp.setWeights( weights );
      }

      double error = calculateErrorsFullSet();
      double testError = calculateErrorTestSet();

      if ( error > 10 * m_error )
      {
         getLogger() << Msg::Error << "Terminated training: Solution is diverging." << Msg::EndReq;
         break;
      }

      m_error = error;
      getLogger() << Msg::Debug << "Training epoch " << iIter << ": error = " << m_error << Msg::EndReq;
      if ( testError != 0 )
      {
         getLogger() << Msg::Debug << " => Test sample performance: " << testError << Msg::EndReq;
      }

      if ( isConverged() )
      {
         break;
      }
   }

   getLogger() << Msg::Verbose << "Batch generation: num proposals = " << m_numSampleProposals << " of which " << m_numFailedSampleProposals << " failed." << Msg::EndReq;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// updateSampleError
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void StochasticGradDescMlpTrainer::updateSampleError( size_t sampleIndex, double error )
{
   m_sampleErrors[ sampleIndex ] = error;
   if ( error > m_largestSampleError )
   {
      m_largestSampleError = error;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Static members
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const size_t StochasticGradDescMlpTrainer::s_numTriesAddSampleToBatch = 1000;

} /// namespace Mva
