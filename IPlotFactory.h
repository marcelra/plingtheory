#ifndef IPLOTFACTORY_H
#define IPLOTFACTORY_H

#include "MarkerDrawAttr.h"
#include "Palette.h"
#include "SingletonBase.h"

#include <cassert>
#include <vector>

#include <QColor>

class Logger;
class RawPcmData;

namespace Math
{
class IAccumArray;
class Regular2DHistogram;
}

namespace WaveAnalysis
{
class StftData;
}

namespace Plotting
{
class Plot2D;
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
      virtual Plotting::Plot2D& createPlot( const std::string& name ) = 0;

   /**
    * Create different types of plots.
    */
   public:
      virtual void createGraph( const std::vector< double >& yData, const QColor& colour = Qt::black );
      virtual void createGraph( const std::vector< double >& xData, const std::vector< double >& yData, const QColor& colour = Qt::black ) = 0;
      virtual void drawPcmData( const RawPcmData& pcmData, const QColor& colour = Qt::black ) = 0;

      virtual void createScatter( const std::vector< double >& xData,
                                  const std::vector< double >& yData,
                                  const Plotting::MarkerDrawAttr& markerDrawAttr = Plotting::MarkerDrawAttr() ) = 0;

      virtual void createZScatter( const std::vector< double >& xData,
                                   const std::vector< double >& yData,
                                   const std::vector< double >& zData,
                                   const Plotting::Palette& palette,
                                   const Plotting::MarkerDrawAttr& markerDrawAttr = Plotting::MarkerDrawAttr() )
      {
         assert( false );
         /// Use params in order to suppress compiler warnings.
         assert( sizeof( xData ) );
         assert( sizeof( yData ) );
         assert( sizeof( zData ) );
         assert( sizeof( palette ) );
         assert( sizeof( markerDrawAttr ) );
      }

      virtual void createStftGraph( const WaveAnalysis::StftData& stftData ) = 0;
      virtual void createHistogram( const Math::IAccumArray& hist, const QColor& colour = Qt::black ) = 0;

      virtual void create2DHist( const Math::Regular2DHistogram& hist,
                                 const Plotting::Palette& palette = Plotting::Palette::heatPalette() )
      {
         assert( &hist ); assert( &palette); assert( false );
      }

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
