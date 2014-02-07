#include "ProgramOptions.h"

#include "Exceptions.h"

#include <iostream>

ProgramOptions::ProgramOptions( int argc, char* argv[] ) :
   m_hasErrors( false ),
   m_doRunTests( false ),
   m_doRunDevelopmentCode( false ),
   m_doProcessRootEvents( true ),
   m_doUseColorLogger( true ),
   m_logFileName( "" )
{
   assert( !s_instance );
   for ( int i = 1; i < argc; ++i )
   {
      m_argList.push_back( argv[i] );
   }
   parseArguments();
}

ProgramOptions::~ProgramOptions()
{}

const ProgramOptions& ProgramOptions::getInstance()
{
   assert( s_instance );
   return *s_instance;
}

void ProgramOptions::parseArguments()
{
   for ( StringList::iterator it = m_argList.begin(); it != m_argList.end(); )
   {
      std::string opt = *it;
      /// Arguments
      if ( opt == "develop" )
      {
         m_doRunDevelopmentCode = true;
      }
      else if ( opt == "testall" )
      {
         m_doRunTests = true;
      }
      /// Long options
      else if ( opt == "--disableroot" )
      {
         m_doProcessRootEvents = false;
      }
      /// Short options
      else if ( opt == "-o" )
      {
         m_doUseColorLogger = false;
         ++it;
         m_logFileName = *it;
      }
      else
      {
         throw ExceptionOptionUnknown( *it );
         break;
      }
      ++it;
   }
}

bool ProgramOptions::hasErrors() const
{
   return m_hasErrors;
}

void ProgramOptions::printOptions( std::ostream& os )
{
   os << "Commands:\n";
   os << "develop             : Run development code.\n";
   os << "testall             : Run all tests.\n";
   os << "\n";
   os << "Options:\n";
   os << "--disableroot       : Do not enter the root event loop.\n";
   os << "                      This causes the application to terminate directly (i.e. without showing graphs etc.)\n";
   os << "-o <filename>       : Write all log data to file with name <filename>\n";
   os.flush();
}

bool ProgramOptions::doRunTests() const
{
   return m_doRunTests;
}

bool ProgramOptions::doRunDevelopmentCode() const
{
   return m_doRunDevelopmentCode;
}

bool ProgramOptions::doProcessRootEvents() const
{
   return m_doProcessRootEvents;
}

bool ProgramOptions::doUseColorLogger() const
{
   return m_doUseColorLogger;
}

const std::string& ProgramOptions::getLogFileName() const
{
   return m_logFileName;
}

ProgramOptions* ProgramOptions::s_instance = 0;
