#ifndef STOCHASTICGRADDESCMLPTRAINER_H
#define STOCHASTICGRADDESCMLPTRAINER_H

#include "MlpTrainer.h"
#include "RandomNumberGenerator.h"

namespace Mva
{

class StochasticGradDescMlpTrainer : public Mva::MlpTrainer
{
   public:
      StochasticGradDescMlpTrainer( MultiLayerPerceptron& mlp, const std::string& algName = "GradDescMlpTrainer", const AlgorithmBase* parent = 0 );

      void setEta( double eta );
      void setBatchSize( size_t batchSize, size_t numIterationsPerEpoch );

      void train();

   private:
      std::vector< size_t > makeBatch();
      double calculateErrorsFullSet();
      void updateSampleError( size_t sampleIndex, double error );

   private:
      double					 m_eta;
      size_t					 m_batchSize;
      size_t					 m_numIterationsPerEpoch;
      RandomNumberGenerator m_rng;
      RealVector				 m_sampleErrors;
      double					 m_largestSampleError;
      const static size_t	 s_numTriesAddSampleToBatch;

   /**
    * Development members.
    */
   private:
      size_t					 m_numSampleProposals;
      size_t					 m_numFailedSampleProposals;
};

} /// namespace Mva

#endif // GRADDESCMLPTRAINER_H
