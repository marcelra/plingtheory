#include "Msg.h"

#include "GlobalLogParameters.h"

#include <cassert>

////////////////////////////////////////////////////////////////////////////////
/// Msg::strRep
////////////////////////////////////////////////////////////////////////////////
const char* Msg::strRep( Msg::LogLevel logLevel )
{
   switch ( logLevel )
   {
      case Msg::Always:
         return "ALWAYS";
         break;
      case Msg::Fatal:
         return "FATAL";
         break;
      case Msg::Error:
         return "ERROR";
         break;
      case Msg::Warning:
         return "WARNING";
         break;
      case Msg::Info:
         return "INFO";
         break;
      case Msg::Debug:
         return "DEBUG";
         break;
      case Msg::Verbose:
         return "VERBOSE";
         break;
      default:
         assert( false );
   }
   return "";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// colorCode
//////////////////////////////////////////////////////////////////////////////////////////////////////
const char* Msg::colorCode( Msg::LogLevel logLevel )
{
   if ( GlobalLogParameters::getInstance().getUseColors() )
   {
      switch ( logLevel )
      {
         case Msg::Always:
            return "\033[40;37m";
            break;
         case Msg::Fatal:
            return "\033[45;30m";
            break;
         case Msg::Error:
            return "\033[41;30m";
            break;
         case Msg::Warning:
            return "\033[43;30m";
            break;
         case Msg::Info:
            return "";
            break;
         case Msg::Debug:
            return "";
            break;
         case Msg::Verbose:
            return "";
            break;
         default:
            assert( false );
      }
   }
   else
   {
      return "";
   }
}

std::string Msg::fmtPlural( size_t n, const std::string& singular, const std::string& plural )
{
   return n==1 ? singular : plural;
}
