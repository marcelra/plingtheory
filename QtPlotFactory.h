#ifndef QTPLOTFACTORY_H
#define QTPLOTFACTORY_H

#include "IPlotFactory.h"

namespace Plotting
{
class Plot2D;
class IPaintItem;
}

namespace Plotting
{
class MarkerDrawAttr;
}

namespace PlotInterface
{

/// TODO: doc
class QtPlotFactory : public IPlotFactory
{
   public:
      static void initialise();

   public:
      void createPlot( const std::string& name );

   public:
      void createGraph( const std::vector< double >& xData, const std::vector< double >& yData, const QColor& colour );
      void drawPcmData( const RawPcmData& pcmData, const QColor& colour );

      void createScatter( const std::vector< double >& xData,
                          const std::vector< double >& yData,
                          const Plotting::MarkerDrawAttr& markerDrawAttr );

      void createStftGraph( const WaveAnalysis::StftData& stftData );
      virtual void createHistogram( const Math::IAccumArray& hist, const QColor& colour );
      virtual void create2DHist( const Math::Regular2DHistogram& hist );

   protected:
      QtPlotFactory();
      virtual ~QtPlotFactory();

   private:
      Plotting::Plot2D*                     m_currentPlot;
      std::vector< Plotting::IPaintItem* >  m_plotItems;
};

} /// namespace PlotInterface

#endif // QTPLOTFACTORY_H
