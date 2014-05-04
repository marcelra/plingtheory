#ifndef REBINNEDSRGRAPH_H
#define REBINNEDSRGRAPH_H

#include <cstddef>
#include <string>

namespace Math
{
class Regular2DHistogram;
}

namespace WaveAnalysis
{
   class StftData;
}

namespace Visualisation
{

/// TODO: doc
class RebinnedSRGraph
{
   public:
      RebinnedSRGraph( const WaveAnalysis::StftData& stftData, size_t nBinsX = 0, size_t nBinsY = 0 );
      virtual ~RebinnedSRGraph();

      void create( const std::string& name );

   private:
      const WaveAnalysis::StftData&  m_stftData;

   private:
      Math::Regular2DHistogram*     m_hist;
      size_t                        m_nBinsX;
      size_t                        m_nBinsY;

   private:
      RebinnedSRGraph( const RebinnedSRGraph& other );
      RebinnedSRGraph& operator=( const RebinnedSRGraph& other );
};

} /// namespace Visualisation

#endif // REBINSRGRAPH_H
