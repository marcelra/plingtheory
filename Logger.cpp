#include "Logger.h"

#include "Exceptions.h"
#include "GlobalLogParameters.h"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////
/// Logger constructor
////////////////////////////////////////////////////////////////////////////////
Logger::Logger( const std::string& name ) :
   m_name( name ),
   m_stream( GlobalLogParameters::getInstance().getStream() ),
   m_currentLevel( Msg::Never ),
   m_threshold( GlobalLogParameters::getInstance().getThreshold() )
{
}

////////////////////////////////////////////////////////////////////////////////
/// Logger copy constructor
////////////////////////////////////////////////////////////////////////////////
Logger::Logger( const Logger& other ) :
  m_name( other.m_name ),
  m_stream( other.m_stream ),
  m_currentLevel( Msg::Never ),
  m_threshold( other.m_threshold )
{}

////////////////////////////////////////////////////////////////////////////////
/// setThreshold
////////////////////////////////////////////////////////////////////////////////
void Logger::setThreshold( Msg::LogLevel threshold )
{
   this->m_threshold = threshold;
}

////////////////////////////////////////////////////////////////////////////////
/// Template specialisation for Msg::LogLevel
////////////////////////////////////////////////////////////////////////////////
template <>
Logger& Logger::operator<<( const Msg::LogLevel& logLevel )
{
   m_currentLevel = logLevel;

   if ( m_currentLevel <= m_threshold )
   {
      size_t nameFieldWidth = GlobalLogParameters::getInstance().getNameFieldWidth();
      size_t levelFieldWidth = GlobalLogParameters::getInstance().getLevelFieldWidth();

      formatInField( m_name, nameFieldWidth );
      m_stream << Msg::colorCode( m_currentLevel );
      formatInField( Msg::strRep( m_currentLevel ), levelFieldWidth );
      if ( GlobalLogParameters::getInstance().getUseColors() )
      {
         m_stream << "\033[0m";
      }
   }

   return *this;
}

////////////////////////////////////////////////////////////////////////////////
/// Template specialisation for Msg::LogCommand
////////////////////////////////////////////////////////////////////////////////
template <>
Logger& Logger::operator<<( const Msg::LogCommand& logCommand )
{
   switch ( logCommand )
   {
      case Msg::EndReq:
         if ( m_currentLevel <= m_threshold )
         {
            m_stream << std::endl;
            m_currentLevel = Msg::Never;
         }
         break;
      default:
         assert( false );
   }
   return *this;
}

////////////////////////////////////////////////////////////////////////////////
/// Template specialisation for vector printing
////////////////////////////////////////////////////////////////////////////////
template<>
Logger& Logger::operator<<( const RealVector& vec )
{
   *this << "(";
   for ( size_t i = 0; i < vec.size() - 1; ++i )
   {
      *this << vec[i] << ", ";
   }
   *this << vec[ vec.size() - 1 ] << ")";
   return *this;
}

////////////////////////////////////////////////////////////////////////////////
/// formatInField
////////////////////////////////////////////////////////////////////////////////
void Logger::formatInField( const std::string& message, size_t lengthOfField )
{
   if ( m_currentLevel <= m_threshold )
   {
      size_t lenMsg = message.size();
      if ( lenMsg < lengthOfField )
      {
         m_stream << message;
         std::string spacer( lengthOfField - lenMsg, ' ' );
         m_stream << spacer;
      }
      else
      {
         std::string croppedMessage = message.substr( 0, lengthOfField - 3 );
         croppedMessage += "...";
         m_stream << croppedMessage;
      }
      std::string spacer( GlobalLogParameters::getInstance().getSpacerWidth(), ' ' );
      m_stream << spacer;
   }
}



////////////////////////////////////////////////////////////////////////////////
/// Global logger
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// Global logger instance
////////////////////////////////////////////////////////////////////////////////
Logger* gp_gLog = 0;

////////////////////////////////////////////////////////////////////////////////
/// Global logger access
////////////////////////////////////////////////////////////////////////////////
Logger& gLog()
{
   return *gp_gLog;
}

////////////////////////////////////////////////////////////////////////////////
/// initGlobalLogger
////////////////////////////////////////////////////////////////////////////////
void initGlobalLogger( Msg::LogLevel threshold, bool doUseColors, const std::string& fileName )
{
   if ( !gp_gLog )
   {
      GlobalLogParameters& globalLogPars = const_cast< GlobalLogParameters& >( GlobalLogParameters::getInstance() );
      globalLogPars.setThreshold( threshold );
      globalLogPars.setUseColors( doUseColors );
      if ( fileName != "" )
      {
         globalLogPars.openFileStream( fileName );
      }

      gp_gLog = new Logger( "gLog" );
      gp_gLog->setThreshold( threshold );
   }
}

void closeGlobalLogger()
{
   if ( gp_gLog )
   {
      // *gp_gLog << Msg::Verbose << "Global logger closing..." << Msg::EndReq;
      delete gp_gLog;
      gp_gLog = 0;
   }
}



////////////////////////////////////////////////////////////////////////////////
/// testLogger
////////////////////////////////////////////////////////////////////////////////
int testLogger()
{
   Logger msg( "My message" );
   msg << Msg::Info << "Test message" << Msg::EndReq;
   msg << Msg::Debug << "Debug message" << Msg::EndReq;
   msg << Msg::Verbose << "Will not be visible" << Msg::EndReq;
   msg << Msg::Fatal << "Always visible" << Msg::EndReq;
   msg << Msg::Error << "An error message" << Msg::EndReq;
   msg << Msg::Warning << "An error message" << Msg::EndReq;
   msg << Msg::Info << &msg << Msg::EndReq;

   Logger longName( "This is a logger with a long name" );
   longName << Msg::Info << "This is a logger with a very long name" << Msg::EndReq;

   gLog() << Msg::Info << "A message from the global logger!" << Msg::EndReq;
   return 0;
}

