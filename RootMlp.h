#ifndef ROOTMLP_H
#define ROOTMLP_H

#include "RealVector.h"

#include <string>

class TMultiLayerPerceptron;
class TTree;

namespace Mva
{

class RootMlp
{
   public:
      RootMlp( const std::string& networkLayout );

      void train( const std::vector< RealVector >& inputData, const std::vector< RealVector >& outputData );
      RealVector evaluate( const RealVector& x );

   private:
      TTree* getTreeFromTrainData( const std::vector< RealVector >& inputData, const std::vector< RealVector >& outputData );

   private:
      std::string                                m_networkLayout;
      std::auto_ptr< TMultiLayerPerceptron >     m_mlp;

};

} /// namespace Mva

#endif // ROOTMLP_H
