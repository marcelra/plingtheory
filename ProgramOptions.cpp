#include "ProgramOptions.h"

#include "Exceptions.h"

#include <cassert>
#include <cstdlib>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ProgramOptions::ProgramOptions( int argc, char* argv[] ) :
   m_hasErrors( false ),
   m_doRunTests( false ),
   m_doRunSingleTest( false ),
   m_doRunAnalyses( false ),
   m_doRunSingleAnalysis( false ),
   m_doRunDevelopmentCode( false ),
   m_doCompareRootFiles( false ),
   m_useRootInterface( false ),
   m_useQtInterface( true ),
   m_doUseColorLogger( true ),
   m_doShowLoggerIds( false ),
   m_useRegressionLogConfig( false ),
   m_rootFileNameOld( "" ),
   m_rootFileNameNew( "" ),
   m_logFileName( "" ),
   m_dataDir( "" ),
   m_rootFileOutput( "" ),
   m_logLevel( 4 )
{
   assert( !s_instance );
   for ( int i = 1; i < argc; ++i )
   {
      m_argList.push_back( argv[i] );
   }
   parseArguments();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ProgramOptions::~ProgramOptions()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getInstance
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const ProgramOptions& ProgramOptions::getInstance()
{
   assert( s_instance );
   return *s_instance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// parseArguments
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ProgramOptions::parseArguments()
{
   for ( StringList::const_iterator it = m_argList.begin(); it != m_argList.end(); )
   {
      std::string opt = *it;
      /// Arguments
      if ( opt == "develop" )
      {
         m_doRunDevelopmentCode = true;
      }
      else if ( opt == "test" )
      {
         m_doRunTests = true;
      }
      else if ( opt == "singletest" )
      {
         m_doRunSingleTest = true;
      }
      else if ( opt == "analyse" )
      {
         m_doRunAnalyses = true;
      }
      else if ( opt == "analysis" )
      {
         m_doRunSingleAnalysis = true;
      }
      else if ( opt == "compare" )
      {
         it = safeAdvanceIter( it, m_argList, "compare" );
         m_rootFileNameOld = *it;
         it = safeAdvanceIter( it, m_argList, "compare" );
         m_rootFileNameNew = *it;
         m_doCompareRootFiles = true;
      }
      /// Short options
      else if ( opt == "-root" )
      {
         m_useRootInterface = true;
         if ( m_useQtInterface )
         {
            throw ExceptionGeneral( "Both Qt and ROOT interface are requested, but these cannot be used together." );
         }
      }
      else if ( opt == "-qt" )
      {
         m_useQtInterface = true;
         if ( m_useRootInterface )
         {
            throw ExceptionGeneral( "Both Qt and ROOT interface are requested, but these cannot be used together." );
         }
      }
      else if ( opt == "-r" )
      {
         it = safeAdvanceIter( it, m_argList, "-r" );
         m_rootFileOutput = *it;
      }
      else if ( opt == "-o" )
      {
         m_doUseColorLogger = false;
         it = safeAdvanceIter( it, m_argList, "-o" );
         m_logFileName = *it;
      }
      /// Long options
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
      else if ( opt.find( "--regression" ) == 0 )
      {
         m_useRegressionLogConfig = true;
      }
      else if ( opt.find( "--no-colours" ) == 0 )
      {
         m_doUseColorLogger = false;
      }
      else if ( opt.find( "--show-id" ) == 0 )
      {
         m_doShowLoggerIds = true;
      }
      else if ( opt == "-v" )
      {
         it = safeAdvanceIter( it, m_argList, "-v" );
         std::string arg = *it;
         int logLevel = atoi( arg.c_str() );
         if ( logLevel < 0 || logLevel > 6 )
         {
            throw ExceptionOptionArgumentParsing( "-v" );
         }
         m_logLevel = logLevel;
      }
      else if ( opt == "-i" )
      {
         std::string arg;
         it = safeAdvanceIter( it, m_argList, "-i" );
         arg = *it;
         int logId = atoi( arg.c_str() );
         it = safeAdvanceIter( it, m_argList, "-i" );
         arg = *it;
         int logLevel = atoi( arg.c_str() );
         if ( logLevel < 0 || logLevel > 6 )
         {
            throw ExceptionOptionArgumentParsing( "-i" );
         }
         m_inspectLogIds[ logId ] = static_cast< Msg::LogLevel >( logLevel );
      }
      else
      {
         throw ExceptionOptionUnknown( *it );
         break;
      }
      ++it;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// hasErrors
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ProgramOptions::hasErrors() const
{
   return m_hasErrors;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// printOptions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ProgramOptions::printOptions( std::ostream& os )
{
   os << "Commands:\n";
   os << "develop             : Run development code.\n";
   os << "test                : Run all tests.\n";
   os << "singletest          : Run single test.\n";
   os << "analyse             : Run all analyses.\n";
   os << "analysis            : Run single analysis.\n";
   os << "compare <old> <new> : Compare root-files.\n";
   os << "\n";
   os << "Options:\n";
   os << "-root               : Use ROOT interface.\n";
   os << "-qt                 : Use Qt interface.\n";
   os << "-o <filename>       : Write all log data to file with name <filename>.\n";
   os << "-r <filename>       : Save all root objects to file with name <filename>.\n";
   os << "-v <level>          : Verbosity level of logger. Range [0,6]: 0 is least verbose, 6 is maximally verbose.\n";
   os << "-i <id> <level>     : Inspect messages from logger with id <id> at verbosity <level>.\n";
   os << "--regression        : Configures the logger for regression output comparison.\n";
   os << "--no-colours        : No colours in log messages.\n";
   os << "--show-id           : Show logger IDs.\n";
   os << "--datadir=<datadir> : Directory in which to look for files that are used by test functions.\n";
   os << "\n\n";
   os.flush();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// doRunTests
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ProgramOptions::doRunTests() const
{
   return m_doRunTests;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// doRunSingleTest
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ProgramOptions::doRunSingleTest() const
{
   return m_doRunSingleTest;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// doRunAnalyses
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ProgramOptions::doRunAnalyses() const
{
   return m_doRunAnalyses;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// doRunSingleAnalysis
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ProgramOptions::doRunSingleAnalysis() const
{
   return m_doRunSingleAnalysis;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// doRunDevelopmentCode
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ProgramOptions::doRunDevelopmentCode() const
{
   return m_doRunDevelopmentCode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// useRootInterface
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ProgramOptions::useRootInterface() const
{
   return m_useRootInterface;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// doCompareRootFiles
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ProgramOptions::doCompareRootFiles() const
{
   return m_doCompareRootFiles;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// doUseColorLogger
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ProgramOptions::doUseColorLogger() const
{
   return m_doUseColorLogger;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// doShowLoggerIds
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ProgramOptions::doShowLoggerIds() const
{
   return m_doShowLoggerIds;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getLogFileName
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const std::string& ProgramOptions::getLogFileName() const
{
   return m_logFileName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getDataDir
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const std::string& ProgramOptions::getDataDir() const
{
   return m_dataDir;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getLogLevel
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ProgramOptions::getLogLevel() const
{
   return m_logLevel;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getRootFileOutput
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const std::string& ProgramOptions::getRootFileOutput() const
{
   return m_rootFileOutput;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getRootFileCompareOld
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const std::string& ProgramOptions::getRootFileNameCompareOld() const
{
   return m_rootFileNameOld;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getRootFileCompareNew
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const std::string& ProgramOptions::getRootFileNameCompareNew() const
{
   return m_rootFileNameNew;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// safeAdvanceIter
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StringList::const_iterator ProgramOptions::safeAdvanceIter( StringList::const_iterator it, const StringList& list, const std::string& currentOption ) const
{
   if ( ++it == list.end() )
   {
      throw ExceptionOptionArgumentParsing( currentOption );
   }
   return it;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// useQtInterface
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ProgramOptions::useQtInterface() const
{
   return m_useQtInterface;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// doUseRegressionLogConfig
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ProgramOptions::doUseRegressionLogConfig() const
{
   return m_useRegressionLogConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getLoggerInspectMap
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const std::map< size_t, Msg::LogLevel >& ProgramOptions::getLoggerInspectMap() const
{
   return m_inspectLogIds;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Singleton instance
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ProgramOptions* ProgramOptions::s_instance = 0;
