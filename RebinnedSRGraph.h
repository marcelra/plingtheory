#ifndef REBINNEDSRGRAPH_H
#define REBINNEDSRGRAPH_H

#include <cstddef>

class TCanvas;
class TH2F;

namespace WaveAnalysis
{
   class StftData;
}

namespace Visualisation
{

class RebinnedSRGraph
{
   public:
      RebinnedSRGraph( const WaveAnalysis::StftData& stftData, size_t nBinsX = 0, size_t nBinsY = 0 );
      virtual ~RebinnedSRGraph();

      TCanvas* create();

      const TCanvas* getCanvas() const;
      TCanvas* getCanvas();

      const TH2F* getGraph() const;
      TH2F* getGraph();

   private:
      const WaveAnalysis::StftData&  m_stftData;

   private:
      TCanvas*  m_canvas;
      TH2F*     m_hist;
      size_t    m_nBinsX;
      size_t    m_nBinsY;

   private:
      RebinnedSRGraph( const RebinnedSRGraph& other );
      RebinnedSRGraph& operator=( const RebinnedSRGraph& other );
};

} /// namespace Visualisation

#endif // REBINSRGRAPH_H
