#ifndef STFTGRAPH_H
#define STFTGRAPH_H

#include "RealVector.h"

#include <cstddef>
#include <map>

class TCanvas;
class TH2F;

namespace WaveAnalysis
{
   class RawStftData;
}

namespace Visualisation
{

/// TODO: document
class StftGraph
{
   public:
      StftGraph( const WaveAnalysis::RawStftData& fftwData );
      virtual ~StftGraph();

      TCanvas* create();

      const TCanvas* getCanvas() const;
      TCanvas* getCanvas();

      const TH2F* getGraph() const;
      TH2F* getGraph();

   private:
      const WaveAnalysis::RawStftData&  m_stftData;

   private:
      TCanvas*  m_canvas;
      TH2F*     m_hist;
};

} /// namespace Visualisation

#endif // STFTGRAPH_H
