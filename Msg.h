#ifndef MSG_H
#define MSG_H

#include <string>

/**
 * Class with commands/manipulators for the Logger class
 * @see Logger
 */
class Msg
{
   public:
      /**
       * LogLevel indicates the importance of a message
       */
      enum LogLevel
      {
         Always,
         Fatal,
         Error,
         Warning,
         Info,
         Debug,
         Verbose,
         Never
      };

      /**
       * Manipulators
       */
      enum LogCommand
      {
         EndReq
      };

      /**
       * Get a string representation for LogLevel
       */
      static const char* strRep( Msg::LogLevel logLevel );
      static const char* colorCode( Msg::LogLevel logLevel );

      /**
       * Format string plural or singular depending on argument.
       * E.g. call with fmtPlural( n, "argument", "arguments" ), will only format "argument" (instead of "arguments") if n is equal to 1.
       */
      static std::string fmtPlural( size_t n, const std::string& singular, const std::string& plural );
};

#endif // MSG_H
