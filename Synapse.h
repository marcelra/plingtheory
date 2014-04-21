#ifndef SYNAPSE_H
#define SYNAPSE_H

namespace Mva
{

class MlpNode;

class Synapse
{
   public:
      Synapse( MlpNode* to, double weight );
      void fire( double value );
      double& getWeightRef();

   private:
      MlpNode*    m_neuron;
      double      m_weight;
};

} /// namespace Mva

#endif // SYNAPSE_H
