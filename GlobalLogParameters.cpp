#include "GlobalLogParameters.h"

#include "Logger.h"

#include <iostream>
#include <fstream>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// theInstance
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GlobalLogParameters* GlobalLogParameters::s_theInstance = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getInstance
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const GlobalLogParameters& GlobalLogParameters::getInstance()
{
   if ( !s_theInstance )
   {
      s_theInstance = new GlobalLogParameters();
   }
   return *s_theInstance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GlobalLogParameters::GlobalLogParameters() :
   m_nameFieldWidth( 30 ),
   m_levelFieldWidth( 10 ),
   m_loggerIdFieldWidth( 10 ),
   m_spacerWidth( 2 ),
   m_threshold( Msg::Verbose ),
   m_useColors( true ),
   m_displayLoggerId( true ),
   m_overrideLocalThresholds( false ),
   m_stream( &std::cout ),
   m_fstream( 0 )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GlobalLogParameters::~GlobalLogParameters()
{
   if ( m_fstream )
   {
      m_fstream->close();
      delete m_fstream;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getNameFieldWidth
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t GlobalLogParameters::getNameFieldWidth() const
{
   return m_nameFieldWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getLevelFieldWidth
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t GlobalLogParameters::getLevelFieldWidth() const
{
   return m_levelFieldWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getLoggerIdFieldWidth
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t GlobalLogParameters::getLoggerIdFieldWidth() const
{
   return m_loggerIdFieldWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getSpaceWidth
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t GlobalLogParameters::getSpacerWidth() const
{
   return m_spacerWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getThreshold
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Msg::LogLevel& GlobalLogParameters::getThreshold() const
{
   return m_threshold;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setThreshold
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GlobalLogParameters::setThreshold( Msg::LogLevel threshold )
{
   m_threshold = threshold;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setRegressionConfig
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GlobalLogParameters::setRegressionConfig()
{
   m_threshold = Msg::Never;
   m_overrideLocalThresholds = true;
   m_displayLoggerId = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setUseColors
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GlobalLogParameters::setUseColors( bool useColors )
{
   m_useColors = useColors;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getUseColors
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GlobalLogParameters::getUseColors() const
{
   return m_useColors;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getStream
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::ostream& GlobalLogParameters::getStream() const
{
   return *m_stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// openFileStream
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GlobalLogParameters::openFileStream( const std::string& fileName )
{
   assert( !m_fstream );
   m_fstream = new std::fstream( fileName.c_str(), std::fstream::out );
   m_stream = m_fstream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// doOverrideLocalThresholds
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GlobalLogParameters::doOverrideLocalThresholds() const
{
   return m_overrideLocalThresholds;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// doDisplayLoggerIds
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GlobalLogParameters::doDisplayLoggerIds() const
{
   return m_displayLoggerId;
}
