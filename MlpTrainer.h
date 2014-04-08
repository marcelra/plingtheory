#ifndef MLPTRAINER_H
#define MLPTRAINER_H

#include "MultilayerPerceptron.h"

namespace Mva
{

class MlpTrainer
{
   public:
      MlpTrainer( Network& network, const std::vector< RealVector >& inputData, const std::vector< RealVector >& outputData );

      void train();

   private:
      Network&                         m_network;
      const std::vector< RealVector >& m_inputData;
      const std::vector< RealVector >& m_outputData;
};

} /// namespace Mva

#endif // MLPTRAINER_H
