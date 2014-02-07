#include "ProgramOptions.h"

#include "Exceptions.h"

#include <cassert>
#include <cstdlib>
#include <iostream>

ProgramOptions::ProgramOptions( int argc, char* argv[] ) :
   m_hasErrors( false ),
   m_doRunTests( false ),
   m_doRunDevelopmentCode( false ),
   m_doProcessRootEvents( true ),
   m_doUseColorLogger( true ),
   m_logFileName( "" ),
   m_dataDir( "" ),
   m_logLevel( 4 )
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
      else if ( opt == "-b" )
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
      else if ( opt.find( "--datadir" ) == 0 )
      {
         std::string dir;
         if ( opt.size() > 9 )
         {
            if ( opt[9] != '=' )
            {
               throw ExceptionOptionArgumentParsing( "--datadir" );
            }
            else
            {
               std::string argInOpt= opt.substr( 10, opt.size() );
               if ( argInOpt.size() == 0 )
               {
                  throw ExceptionOptionArgumentParsing( "--datadir" );
               }
               dir = argInOpt;
            }
         }
         else
         {
            throw ExceptionOptionArgumentParsing( "--datadir" );
         }
         m_dataDir = dir;
      }
      else if ( opt == "-v" )
      {
         ++it;
         std::string arg = *it;
         int logLevel = atoi( arg.c_str() );
         if ( logLevel < 0 || logLevel > 6 )
         {
            throw ExceptionOptionArgumentParsing( "-v" );
         }
         m_logLevel = logLevel;
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
   os << "-b                  : Do not enter the root event loop.\n";
   os << "                      This causes the application to terminate directly (i.e. without showing graphs etc.).\n";
   os << "-o <filename>       : Write all log data to file with name <filename>.\n";
   os << "-v <level>          : Verbosity level of logger. Range [0,6]: 0 is least verbose, 6 is maximally verbose.\n";
   os << "--datadir=<datadir> : Directory in which to look for files that are used by test functions.\n";
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

const std::string& ProgramOptions::getDataDir() const
{
   return m_dataDir;
}

int ProgramOptions::getLogLevel() const
{
   return m_logLevel;
}

ProgramOptions* ProgramOptions::s_instance = 0;
