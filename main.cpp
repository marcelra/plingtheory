/// Framework classes
#include "Exceptions.h"
#include "GlobalLogParameters.h"
#include "Logger.h"
#include "ProgramOptions.h"
#include "RootUtilities.h"
#include "SingletonStore.h"
#include "TestSuite.h"

/// STL includes
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
/// Forward declarations
////////////////////////////////////////////////////////////////////////////////
int runTests( const ProgramOptions* programOptions );
int finaliseApplication( int exitCode );
void printUsageAndExit();

////////////////////////////////////////////////////////////////////////////////
/// Singal handlers
////////////////////////////////////////////////////////////////////////////////
void handleSIGINT( int /* signal */ )
{
   std::cout << std::endl;
   gLog() << Msg::Verbose << "Caught interrupt" << Msg::EndReq;
   exit( finaliseApplication( 0 ) );
}

////////////////////////////////////////////////////////////////////////////////
/// Initialisation
////////////////////////////////////////////////////////////////////////////////
const ProgramOptions* initialiseApplication( int argc, char* argv[] )
{
   std::cout << "Intialising the application..." << std::endl;

   /// Parse program options
   ProgramOptions* programOptions = new ProgramOptions( argc, argv );

   /// Setup logger
   initGlobalLogger( Msg::Verbose );
   Msg::LogLevel threshold = GlobalLogParameters::getInstance().getThreshold();
   gLog() << Msg::Always << "Global logger online; message threshold is " << Msg::strRep( threshold ) << Msg::EndReq;

   /// Setup ROOT
   RootUtilities::initRootApplication();

   /// Handle interrupt signal
   struct sigaction sigIntHandler;
   sigIntHandler.sa_handler = &handleSIGINT;
   sigemptyset( &sigIntHandler.sa_mask );
   sigIntHandler.sa_flags = 0;
   sigaction( SIGINT, &sigIntHandler, 0 );

   gLog() << Msg::Verbose << "Signal handler active" << Msg::EndReq;

   /// Report finalisation complete
   gLog() << Msg::Info << "Application initialised" << Msg::EndReq;
   return programOptions;
}

////////////////////////////////////////////////////////////////////////////////
/// Finalisation
////////////////////////////////////////////////////////////////////////////////
int finaliseApplication( int exitCode )
{
   /// Display error if exit code is not equal to zero
   if (  exitCode != 0 )
   {
      gLog() << Msg::Error << "Application exited with code " << exitCode << Msg::EndReq;
   }

   gLog() << Msg::Info << "Finalising..." << Msg::EndReq;

   /// Clear singletons
   delete &SingletonStore::getInstance();

   gLog() << Msg::Info << "Bye." << Msg::EndReq;

   /// Close global logger (special singleton)
   closeGlobalLogger();

   /// This is the exit point if ROOT is running
   RootUtilities::closeRootAppIfRunning( exitCode );

   /// Exit
   return exitCode;
}

////////////////////////////////////////////////////////////////////////////////
/// Main program
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char* argv[] )
{
   /// Initialise application and parse program options
   const ProgramOptions* programOptions = initialiseApplication( argc, argv );

   if ( programOptions->hasErrors() )
   {
      printUsageAndExit();
   }

   /// Run tests
   int exitCode = runTests( programOptions );
   if ( exitCode == 0 )
   {
      std::cout << "Press CTRL+C to end...";
      std::cout.flush();
   }
   else
   {
      finaliseApplication( exitCode );
   }

   /// Finalisation
   bool doProcessRootEvents = programOptions->doProcessRootEvents();
   delete programOptions;

   if ( doProcessRootEvents )
   {
      RootUtilities::processRootEvents();
   }
   return finaliseApplication( exitCode );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// printUsageAndExit
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void printUsageAndExit()
{
   std::cout << "Usage: ./Suite [options]\n";
   std::cout << "\n";
   std::cout << "Valid options are:\n";
   std::cout << "--disableRoot         : Do not run the root-event loop.\n";
   std::cout << "                        This causes the application to quit immediately after processing.\n";
   std::cout << std::endl;
   finaliseApplication( 1 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Test suite
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int runTests( const ProgramOptions* programOptions )
{
   assert( programOptions );
   int exitCode = 0;
   try
   {
      TestSuite::runCurrentDevelopmentTest();
   }
   catch ( ExceptionTestFailed exc )
   {
      gLog() << Msg::Fatal << "Test failed!" << Msg::EndReq;
      gLog() << Msg::Error << exc << Msg::EndReq;
      exitCode = 2;
      finaliseApplication( exitCode );
   }
   catch ( const BaseException& exc )
   {
      gLog() << Msg::Fatal << "Test failed!" << Msg::EndReq;
      gLog() << Msg::Error << exc << Msg::EndReq;
      exitCode = 1;
      finaliseApplication( exitCode );
   }
   catch ( ... )
   {
      gLog() << Msg::Fatal << "Test failed!" << Msg::EndReq;
      exitCode = 1;
      finaliseApplication( exitCode );
   }
   gLog() << Msg::Info << "Running tests successful." << Msg::EndReq;
   return exitCode;
}
