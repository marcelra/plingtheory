#ifndef MLPTRAINER_H
#define MLPTRAINER_H

#include "RealVector.h"

namespace Mva
{
class Mlp2;
}

namespace Mva
{

class MlpTrainer
{
   public:
      MlpTrainer( Mlp2& mlp );
      virtual ~MlpTrainer();

      void setInputData( const std::vector< RealVector >& inputData, const std::vector< RealVector >& outputData );
      void setNumIterations( size_t nIter );

      virtual void train() = 0;

   protected:
      Mlp2&											m_mlp;
      const std::vector< RealVector >*		m_trainDataInput;
      const std::vector< RealVector >*  	m_trainDataOutput;
      size_t										m_numIterations;
};

} /// namespace Mva

#endif // MLPTRAINER_H
