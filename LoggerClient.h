#ifndef LOGGERCLIENT_H
#define LOGGERCLIENT_H

#include "Msg.h"

#include <memory>

typedef size_t LoggerClientId;

class Logger;

class LoggerClient
{
   public:
      LoggerClient( const std::string& name );
      virtual ~LoggerClient();

      Logger& getLogger() const;
      void setLoggerThreshold( Msg::LogLevel logLevel );

      void setLoggerThreshold( Msg::LogLevel logLevel, LoggerClientId id );
      LoggerClientId getLoggerClientId() const;

   private:
      std::auto_ptr< Logger >    m_logger;
      LoggerClientId             m_loggerClientId;
      static LoggerClientId      s_loggerClientId;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods LoggerClient
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getLogger
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline Logger& LoggerClient::getLogger() const
{
   return *m_logger;
}


#endif // LOGGERCLIENT_H
