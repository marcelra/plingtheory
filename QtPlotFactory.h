#ifndef QTPLOTFACTORY_H
#define QTPLOTFACTORY_H

#include "IPlotFactory.h"

namespace Plotting
{
class Plot2D;
class IPaintItem;
}

namespace PlotInterface
{

class QtPlotFactory : public IPlotFactory
{
   public:
      static void initialise();

   public:
      void createPlot( const std::string& name );

   public:
      void createGraph( const std::vector< double >& xData, const std::vector< double >& yData, const QColor& colour );
      void drawPcmData( const RawPcmData& pcmData, const QColor& colour );

      void createScatter( const std::vector< double >& xData, const std::vector< double >& yData, const QColor& colour );

   protected:
      QtPlotFactory();
      virtual ~QtPlotFactory();

   private:
      Plotting::Plot2D*                     m_currentPlot;
      std::vector< Plotting::IPaintItem* >  m_plotItems;
};

} /// namespace PlotInterface

#endif // QTPLOTFACTORY_H
