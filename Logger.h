#ifndef LOGGER_H
#define LOGGER_H

#include "Msg.h"

#include <string>
#include <ostream>

#include "RealVector.h"

typedef size_t LoggerId;

/**
 * @class Logger
 * @brief Class that provides pretty print messages
 *
 * Typical usage:
 * loggerInstance << Msg::<Level> << <My message> << Msg::EndReq;
 * this will format the output like:
 * TheLoggerName         LEVEL     MyMessage
 *
 * The Logger is sort of thread safe. A mutex lock is created when logger << Msg::<Level> is called. This lock is lifted when
 * logger << Msg::EndReq is called. If the EndReq log commands is forgotten, a deadlock can occur.
 */
class Logger
{
   public:
      /**
       * Constructor
       * @param name: the name of the logger (will be displayed in log messages).
       */
      Logger( const std::string& name );

      /**
       * Copy constructor
       */
      Logger( const Logger& other );

      /**
       * Destructor.
       */
      ~Logger();

      /**
       * Stream operator.
       */
      template < class T >
      Logger& operator<<( const T& x );

      /**
       * Set threshold for this logger instance to @param threshold.
       */
      void setThreshold( Msg::LogLevel threshold );

      /**
       * Sets a common prefix for all the messages from this logger.
       */
      void setMessagePrefix( const std::string& prefix );

      /**
       * Get Logger ID
       */
      LoggerId getLoggerId() const;

      /**
       * Get the name of the logger.
       */
      const std::string& getName() const;

   private:
      /**
       * format message in field with length lengthOfField
       * @param message: the message to be displayed
       * @param lengthOfField: the field width
       */
      void formatInField( const std::string& message, size_t lengthOfField );

   private:
      static LoggerId         s_loggerId;              //! Counts the number of logger instantiations
      LoggerId                m_loggerId;              //! The ID of the current logger

   private:
      std::string             m_name;                  //! the name of the logger
      std::string             m_prefix;                //! prefix for all messages
      std::ostream&           m_stream;                //! the ostream (usually std::cout)
      Msg::LogLevel           m_currentLevel;          //! importance level of current message
      Msg::LogLevel           m_threshold;             //! threshold for displayed messages

};

namespace Music
{
class Note;
}

std::ostream& operator<<( std::ostream& os, const Music::Note& note );

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
Logger& Logger::operator<<( const std::vector< size_t >& indexVector );
template<>
Logger& Logger::operator<<( const RealVector& vec );
template <>
Logger& Logger::operator<<( const Msg::LogLevel& logLevel );
template <>
Logger& Logger::operator<<( const Msg::LogCommand& logCommand );

/// Access global Logger.
Logger& gLog();

/// Initialise global logger (void ptr is used to keep include section clean)
void initGlobalLogger( int threshold, const void* inspectMapPtr, bool doUseColors, bool doDisplayLoggerId, const std::string& fileName );
void initRegressionLogger( const std::string& fileName );

/// Close the global logger
void closeGlobalLogger();

#endif // LOGGER_H
