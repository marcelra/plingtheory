#ifndef STFTGRAPH_H
#define STFTGRAPH_H

#include "RealVector.h"

#include <cstddef>
#include <map>

class TCanvas;
class TH2F;

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

/// TODO: document
class StftGraph
{
   public:
      StftGraph( const WaveAnalysis::StftData& fftwData );
      virtual ~StftGraph();

      void create( const std::string& name );

   private:
      const WaveAnalysis::StftData&  m_stftData;

   private:
      Math::Regular2DHistogram*  m_hist;

   private:
      StftGraph( const StftGraph& other );
      StftGraph& operator=( const StftGraph& other );
};

} /// namespace Visualisation

#endif // STFTGRAPH_H
