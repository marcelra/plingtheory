#include "IPlotFactory.h"

#include "Logger.h"

namespace PlotInterface
{

IPlotFactory::IPlotFactory( const std::string& concreteFactoryTypeName ) :
   SingletonBase( concreteFactoryTypeName )
{
   m_logger = new Logger( concreteFactoryTypeName );
   getLogger() << Msg::Debug << "Initialising the plot factory." << Msg::EndReq;
}

IPlotFactory::~IPlotFactory()
{
   getLogger() << Msg::Debug << "Shutting down..." << Msg::EndReq;
   delete m_logger;
}

IPlotFactory& IPlotFactory::getInstance()
{
   assert( s_instance );
   return *s_instance;
}

IPlotFactory* IPlotFactory::s_instance = 0;

void IPlotFactory::createGraph( const std::vector< double >& yData, const QColor& colour )
{
   std::vector< double > xData( yData.size() );
   for ( size_t i = 0; i < xData.size(); ++i )
   {
      xData[ i ] = i;
   }
   createGraph( xData, yData, colour );
}

} /// namespace PlotInterface
