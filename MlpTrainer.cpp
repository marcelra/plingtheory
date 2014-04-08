#include "MlpTrainer.h"

#include "Logger.h"

namespace Mva
{

MlpTrainer::MlpTrainer( Network& network, const std::vector< RealVector >& inputData, const std::vector< RealVector >& outputData ) :
   m_network( network ),
   m_inputData( inputData ),
   m_outputData( outputData )
{}

void MlpTrainer::train()
{
   Logger msg( "MlpTrainer::train" );
   msg << Msg::Info << "In train..." << Msg::EndReq;

   std::vector< double* > weightRefVec = m_network.getWeights();

   size_t numIterMax = 1;
   size_t numNets = 500;
   for ( size_t iIter = 0; iIter < numIterMax; ++iIter )
   {
      for ( size_t iNet = 0; iNet < numNets; ++iNet )
      {
         double err = 0;
         for ( size_t iData = 0; iData < m_inputData.size(); ++iData )
         {
            const RealVector& outputVec = m_network.eval( m_inputData[ iData ] );
            for ( size_t iOutput = 0; iOutput < outputVec.size(); ++iOutput )
            {
               double diff = outputVec[ iOutput ] - m_outputData[ iData ][ iOutput ];
               err += diff * diff;
            }
         }
      }
   }
}

} /// namespace Mva
