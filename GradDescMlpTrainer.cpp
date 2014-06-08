#include "GradDescMlpTrainer.h"

#include "Logger.h"
#include "Mlp2.h"
#include "Utils.h"

namespace Mva
{

GradDescMlpTrainer::GradDescMlpTrainer( Mlp2& mlp, const std::string& algName, const AlgorithmBase* parent ) :
   MlpTrainer( mlp, algName, parent ),
   m_eta( 0.01 ),
   m_batchSize( -1 ),
   m_rng( 64293 ),
   m_numIterationsPerEpoch( 1 )
{}

void GradDescMlpTrainer::setEta( double eta )
{
   m_eta = eta;
}

void GradDescMlpTrainer::setBatchSize( size_t batchSize, size_t numIterationsPerEpoch )
{
   m_batchSize = batchSize;
   m_numIterationsPerEpoch = numIterationsPerEpoch;
}

std::vector< size_t > GradDescMlpTrainer::makeBatch( bool makeFullBatch )
{
   std::vector< size_t > result;
   if ( m_batchSize == 0 || makeFullBatch )
   {
      result = Utils::createRange( 0, m_trainDataInput->size() );
   }
   else
   {
      result.resize( m_batchSize );
      for ( size_t i = 0; i < m_batchSize; ++i )
      {
         result.push_back( m_rng.uniform( 0, m_trainDataInput->size() ) );
      }
   }
   return result;
}

void GradDescMlpTrainer::train()
{
   assert( m_trainDataInput );

   RealVector&& weights = m_mlp.getWeights();

   for ( size_t iIter = 0; iIter < m_numIterations; ++iIter )
   {

      bool isFinalisingEpoch = iIter % m_numIterationsPerEpoch == 0 ? true : false;

      RealVector gradient( weights.size(), 0 );
      double error = 0;
      std::vector< size_t >&& batch = makeBatch( isFinalisingEpoch );

      for ( size_t iSample = 0; iSample < batch.size(); ++iSample )
      {
         RealVector sampleGradient;
         double sampleError;
         m_mlp.calcErrorAndGradient( (*m_trainDataInput)[ batch[ iSample ] ], (*m_trainDataOutput)[ batch[ iSample ] ], sampleError, sampleGradient );
         gradient = gradient + sampleGradient;
         error += sampleError;
      }

      gradient = gradient * ( 1.0 / batch.size() );
      error /= batch.size();

      if ( isFinalisingEpoch )
      {
         if ( error > 10 * m_error )
         {
            getLogger() << Msg::Error << "Terminated training: Solution is diverging." << Msg::EndReq;
            break;
         }

         m_error = error;
         getLogger() << Msg::Verbose << "Training epoch " << iIter / m_numIterationsPerEpoch << ": error = " << m_error << Msg::EndReq;

         if ( isConverged() )
         {
            break;
         }
      }

      weights = weights - gradient * m_eta;
      m_mlp.setWeights( weights );
   }
}

} /// namespace Mva
