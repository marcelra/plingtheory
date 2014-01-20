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
};

#endif // MSG_H
