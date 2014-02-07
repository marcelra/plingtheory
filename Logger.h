#ifndef LOGGER_H
#define LOGGER_H

#include "Msg.h"

#include <string>
#include <ostream>

#include "RealVector.h"

/**
 * @class Logger
 * @brief Class that provides pretty print messages
 *
 * Typical usage:
 * loggerInstance << Msg::<Level> << <My message> << Msg::EndReq;
 * this will format the output like:
 * TheLoggerName         LEVEL     MyMessage
 */
class Logger
{
   public:
      /**
       * Constructor
       * @param name: the name of the logger (will be displayed in log messages)
       */
      Logger( const std::string& m_name );

      /**
       * Copy constructor
       */
      Logger( const Logger& other );

      /**
       * Stream operator
       */
      template < class T >
      Logger& operator<<( const T& x );

      /**
       * Set threshold for this logger instance to @param threshold
       */
      void setThreshold( Msg::LogLevel m_threshold );

   private:
      /**
       * format message in field with length lengthOfField
       * @param message: the message to be displayed
       * @param lengthOfField: the field width
       */
      void formatInField( const std::string& message, size_t lengthOfField );

      std::string             m_name;                  //! the name of the logger
      std::ostream&           m_stream;                //! the ostream (usually std::cout)
      Msg::LogLevel           m_currentLevel;          //! importance level of current message
      Msg::LogLevel           m_threshold;             //! threshold for displayed messages
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// template operations
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < class T >
Logger& Logger::operator<<( const T& x )
{
   if ( m_currentLevel <= m_threshold )
   {
      m_stream << x;
   }
   return *this;
}

template<>
Logger& Logger::operator<<( const RealVector& vec );
template <>
Logger& Logger::operator<<( const Msg::LogLevel& logLevel );
template <>
Logger& Logger::operator<<( const Msg::LogCommand& logCommand );


/// Access global Logger.
Logger& gLog();

/// Initialise global logger
void initGlobalLogger( Msg::LogLevel threshold, bool doUseColors, const std::string& fileName );

/// Close the global logger
void closeGlobalLogger();

/// Test method
int testLogger();


#endif // LOGGER_H
