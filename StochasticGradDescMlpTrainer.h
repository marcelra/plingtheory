#ifndef STOCHASTICGRADDESCMLPTRAINER_H
#define STOCHASTICGRADDESCMLPTRAINER_H

#include "MlpTrainer.h"
#include "RandomNumberGenerator.h"

namespace Mva
{

/**
 * @class StochasticGradDescMlpTrainer
 * @brief Stochastic/batch training for MLP neural network.
 */
class StochasticGradDescMlpTrainer : public Mva::MlpTrainer
{
   public:
      /**
       * Constructur. Train MLP @param mlp.
       */
      StochasticGradDescMlpTrainer( MultiLayerPerceptron& mlp, const std::string& algName = "GradDescMlpTrainer", const AlgorithmBase* parent = 0 );

      /**
       * Set the gradient descent eta: x(i+1) = x(i) + eta * df/dx(i).
       */
      void setEta( double eta );
      /**
       * Set the batch size and the number of iterations before a full batch study is done.
       */
      void setBatchSize( size_t batchSize, size_t numIterationsPerEpoch );

      /**
       * Train the network.
       */
      void train();

   private:
      /**
       * Make a batch.
       */
      std::vector< size_t > makeBatch();
      /**
       * Calculate the errors of the individual samples for the full set and @return the total error normalised to the number of samples in the full set.
       */
      double calculateErrorsFullSet();
      /**
       * Update the error, @param error, for sample @param sampleIndex.
       */
      void updateSampleError( size_t sampleIndex, double error );

   private:
      double					 m_eta;								//! Gradient descent change coefficient.
      size_t					 m_batchSize;						//! Stochasitically varying batch.
      size_t					 m_numIterationsPerEpoch;		//! Number of iterations before full batch study is done.
      RandomNumberGenerator m_rng;								//! Random number generator.
      RealVector				 m_sampleErrors;					//! Errors for each sample.
      double					 m_largestSampleError;			//! Largest sample error.
      const static size_t	 s_numTriesAddSampleToBatch;	//! Max number of tries to draw an individual sample.

   /**
    * Development members (track sample draw selection efficiency).
    */
   private:
      size_t					 m_numSampleProposals;
      size_t					 m_numFailedSampleProposals;
};

} /// namespace Mva

#endif // GRADDESCMLPTRAINER_H
