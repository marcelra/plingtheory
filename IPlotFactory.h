#ifndef IPLOTFACTORY_H
#define IPLOTFACTORY_H

#include "SingletonBase.h"

#include <vector>

#include <QColor>

class Logger;
class RawPcmData;

namespace Math
{
class IAccumArray;
}

namespace WaveAnalysis
{
class StftData;
}

namespace PlotInterface
{

/**
 * @class IPlotFactory
 * @brief Abstract factory pattern for building plots. The concrete plot factory has to be initialised before it can be used.
 *
 * New plots are created using the createPlot method. This creates a totally new plot. Plot items can be added by calling
 * subsequently any of the dedicated methods.
 */
class IPlotFactory : SingletonBase
{
   public:
      /**
       * Access singleton instance.
       */
      static IPlotFactory& getInstance();

   public:
      /**
       * Create new two dimensional plot canvas.
       */
      virtual void createPlot( const std::string& name ) = 0;

   /**
    * Create different types of plots.
    */
   public:
      virtual void createGraph( const std::vector< double >& yData, const QColor& colour = Qt::black );
      virtual void createGraph( const std::vector< double >& xData, const std::vector< double >& yData, const QColor& colour = Qt::black ) = 0;
      virtual void drawPcmData( const RawPcmData& pcmData, const QColor& colour = Qt::black ) = 0;

      virtual void createScatter( const std::vector< double >& xData, const std::vector< double >& yData, const QColor& colour = Qt::black ) = 0;
      virtual void createStftGraph( const WaveAnalysis::StftData& stftData ) = 0;
      virtual void createHistogram( const Math::IAccumArray& hist ) = 0;

   protected:
      /**
       * Create the factory. @param concreateFactoryTypeName is needed for unique registration in the singleton store.
       */
      IPlotFactory( const std::string& concreteFactoryTypeName );
      /**
       * Virtual destructor.
       */
      virtual ~IPlotFactory();

   protected:
      /**
       * Access the logger.
       */
      Logger& getLogger() const;
      static IPlotFactory*    s_instance;       //! Singleton instance.

   private:
      mutable Logger*         m_logger;         //! the logger should be instantiated by the derived class in the constructor.
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

/**
 * Access plot factory without too much typing.
 */
inline PlotInterface::IPlotFactory& gPlotFactory()
{
   return PlotInterface::IPlotFactory::getInstance();
}

#endif // IPLOTFACTORY_H
