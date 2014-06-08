#ifndef GRADDESCMLPTRAINER_H
#define GRADDESCMLPTRAINER_H

#include "MlpTrainer.h"
#include "RandomNumberGenerator.h"

namespace Mva
{

class GradDescMlpTrainer : public Mva::MlpTrainer
{
   public:
      GradDescMlpTrainer( Mlp2& mlp, const std::string& algName = "GradDescMlpTrainer", const AlgorithmBase* parent = 0 );

      void setEta( double eta );
      void setBatchSize( size_t batchSize, size_t numIterationsPerEpoch );

      void train();

   private:
      std::vector< size_t > makeBatch( bool makeFullBatch );

   private:
      double					 m_eta;
      int						 m_batchSize;
      RandomNumberGenerator m_rng;
      size_t					 m_numIterationsPerEpoch;
};

} /// namespace Mva

#endif // GRADDESCMLPTRAINER_H
