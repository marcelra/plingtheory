#ifndef ROOTPLOTFACTORY_H
#define ROOTPLOTFACTORY_H

#include "IPlotFactory.h"

#include <map>

class TCanvas;

namespace PlotInterface
{

/**
 * @class RootPlotFactory
 */
/// TODO: doc
class RootPlotFactory : public IPlotFactory
{
   public:
      static void initialise();

   public:
      void createPlot( const std::string& name );

   public:
      void createGraph( const std::vector< double >& xData, const std::vector< double >& yData, const QColor& colour );
      void drawPcmData( const RawPcmData& pcmData, const QColor& colour );

      void createScatter( const std::vector< double >& xData, const std::vector< double >& yData, const QColor& colour );
      void createStftGraph( const WaveAnalysis::StftData& stftData );

   private:
      bool checkIsNewCanvas();
      int rootColorFromQColor( const QColor& colour );

   protected:
      RootPlotFactory();
      virtual ~RootPlotFactory();

   private:
      std::map< std::string, TCanvas* >      m_plots;
      TCanvas*                               m_currentCanvas;
};

} /// namespace PlotInterface

#endif // ROOTPLOTFACTORY_H
