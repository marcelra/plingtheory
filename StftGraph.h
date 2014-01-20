#ifndef STFTGRAPH_H
#define STFTGRAPH_H

#include "RealVector.h"

#include <cstddef>
#include <map>

class TCanvas;
class TH2F;

namespace WaveAnalysis
{
   class StftAlgorithm;
}

namespace Visualisation
{

/// TODO: document
class StftGraph
{
   public:
      StftGraph( const WaveAnalysis::StftAlgorithm& fftwAlg );
      virtual ~StftGraph();

      TCanvas* create();

      const TCanvas* getCanvas() const;
      TCanvas* getCanvas();

      const TH2F* getGraph() const;
      TH2F* getGraph();

   private:
      const WaveAnalysis::StftAlgorithm&  m_stftAlg;

   private:
      TCanvas*  m_canvas;
      TH2F*     m_hist;
};

} /// namespace Visualisation

#endif // STFTGRAPH_H
