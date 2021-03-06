#ifndef MLPTRAINER_H
#define MLPTRAINER_H

#include "AlgorithmBase.h"
#include "RealVector.h"

namespace Mva
{
class MultiLayerPerceptron;
}

namespace Mva
{

class MlpTrainer : public AlgorithmBase
{
   public:
      MlpTrainer( MultiLayerPerceptron& mlp, const std::string& algName = "MlpTrainer", const AlgorithmBase* parent = 0 );
      virtual ~MlpTrainer();

      void setInputData( const std::vector< RealVector >& inputData, const std::vector< RealVector >& outputData );
      void setTestData( const std::vector< RealVector >& inputData, const std::vector< RealVector >& outputData );

      void setNumIterations( size_t nIter );
      void setErrorTolerance( double errorTolerance );

      bool isConverged() const;

      virtual void train() = 0;

   protected:
      double calculateErrorTestSet();

   protected:
      MultiLayerPerceptron&					m_mlp;

      const std::vector< RealVector >*		m_trainDataInput;
      const std::vector< RealVector >*  	m_trainDataOutput;

      const std::vector< RealVector >* 	m_testDataInput;
      const std::vector< RealVector >* 	m_testDataOutput;

      double 										m_error;
      double										m_errorTest;

      size_t										m_numIterations;
      double 										m_errorTolerance;
};

} /// namespace Mva

#endif // MLPTRAINER_H
