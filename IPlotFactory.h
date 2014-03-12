#ifndef IPLOTFACTORY_H
#define IPLOTFACTORY_H

#include "SingletonBase.h"

#include <vector>

#include <QColor>

/// TODO: remove when all virtual functions are implemented.
#include <cassert>

class Logger;
class RawPcmData;

namespace WaveAnalysis
{
class StftData;
}

namespace PlotInterface
{

/**
 * @class IPlotFactory
 */
/// TODO: doc
class IPlotFactory : SingletonBase
{
   public:
      static IPlotFactory& getInstance();

   public:
      virtual void createPlot( const std::string& name ) = 0;

   public:
      virtual void createGraph( const std::vector< double >& yData, const QColor& colour = Qt::black );
      virtual void createGraph( const std::vector< double >& xData, const std::vector< double >& yData, const QColor& colour = Qt::black ) = 0;
      virtual void drawPcmData( const RawPcmData& pcmData, const QColor& colour = Qt::black ) = 0;

      virtual void createScatter( const std::vector< double >& xData, const std::vector< double >& yData, const QColor& colour = Qt::black ) = 0;
      virtual void createStftGraph( const WaveAnalysis::StftData& stftData ) = 0;

   protected:
      IPlotFactory( const std::string& concreteFactoryTypeName );
      virtual ~IPlotFactory();

   protected:
      Logger& getLogger() const;
      static IPlotFactory*    s_instance;

   private:
      mutable Logger*         m_logger;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods IPlotFactory
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getLogger
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline Logger& IPlotFactory::getLogger() const
{
   return *m_logger;
}

} /// PlotInterface

inline PlotInterface::IPlotFactory& gPlotFactory()
{
   return PlotInterface::IPlotFactory::getInstance();
}

#endif // IPLOTFACTORY_H
