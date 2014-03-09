/// Framework classes
#include "DevGui.h"
#include "DevSuite.h"
#include "Exceptions.h"
#include "GlobalLogParameters.h"
#include "GlobalParameters.h"
#include "Logger.h"
#include "MainWindow.h"
#include "ProgramOptions.h"
#include "RootFileCompare.h"
#include "RootUtilities.h"
#include "SingletonStore.h"
#include "TestSuite.h"

/// STL includes
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

/// Qt includes
#include <QApplication>

#define DEBUG_APPLICATION 0

#ifdef DEBUG_APPLICATION
#define DBG_MSG( x ) if ( DEBUG_APPLICATION ) std::cout << "### DBG_MSG: " << x << std::endl;
#else
#define DBG_MSG( x )
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Forward declarations
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void runTests( const ProgramOptions* programOptions );
void runDevelopmentCode( const ProgramOptions* programOptions );
void saveRootFileOutput( const ProgramOptions* programOptions );
void compareRootFiles( const ProgramOptions* programOptions );
void finaliseApplication();
void printUsageAndExit();

enum ProgramStatus
{
   PS_OK = 0,
   PS_UNRECOVERABLE_USER_EXCEPTION,
   PS_TEST_FAILED,
   PS_UNRECOVERABLE_INTERNAL_EXCEPTION,
   PS_UNCAUGHT_INTERNAL_EXCEPTION,
   PS_GUI_NONZERO_STATUSCODE
} programStatus;

QApplication* gApp = 0;

class StopExecutionException {};
class ImmediateStopExecutionException {};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Singal handlers
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void handleSIGINT( int /* signal */ )
{
   std::cout << std::endl;
   DBG_MSG( "Caught interrupt" );
   finaliseApplication();
   exit( programStatus );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Initialisation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const ProgramOptions* initialiseApplication( int argc, char* argv[] )
{
   DBG_MSG( "Entering initialiseApplication..." );
   try
   {
      /// Parse program options
      ProgramOptions* programOptions = 0;
      try
      {
         programOptions = new ProgramOptions( argc, argv );
      }
      catch ( const BaseException& exception )
      {
         std::cout << exception.getMessage() << std::endl;

         std::cout << "\nUsage: " << GlobalParameters::getProgramName() << " [options] [commands]\n";
         std::cout << "\n";
         ProgramOptions::printOptions( std::cout );
         return 0;
      }

      /// Setup global parameters
      if ( programOptions->getDataDir() != "" )
      {
         GlobalParameters::setTestDataDir( programOptions->getDataDir() );
      }

      /// Setup logger
      initGlobalLogger( programOptions->getLogLevel(), programOptions->doUseColorLogger(), programOptions->getLogFileName() );
      Msg::LogLevel threshold = GlobalLogParameters::getInstance().getThreshold();

      DBG_MSG( "Global logger initialised; message threshold is " << Msg::strRep( threshold ) );

      /// Setup ROOT
      if ( programOptions->doProcessRootEvents() )
      {
         RootUtilities::initRootApplication();

         /// Handle interrupt signal
         struct sigaction sigIntHandler;
         sigIntHandler.sa_handler = &handleSIGINT;
         sigemptyset( &sigIntHandler.sa_mask );
         sigIntHandler.sa_flags = 0;
         sigaction( SIGINT, &sigIntHandler, 0 );
      }

      /// Report finalisation complete
      gLog() << Msg::Verbose << "Application initialised" << Msg::EndReq;
      DBG_MSG( "Leaving initialiseApplication" );

      gLog() << Msg::Info << "Initialisation complete." << Msg::EndReq;
      return programOptions;
   }
   catch ( BaseException& exc )
   {
      std::cerr << "Exception raised during initialisation!" << std::endl;
      std::cerr << exc << std::endl;
      programStatus = PS_UNRECOVERABLE_INTERNAL_EXCEPTION;
      throw ImmediateStopExecutionException();
      return 0;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Finalisation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void finaliseApplication()
{
   DBG_MSG( "Entering finaliseApplication..." );
   try
   {
      /// Display error if exit code is not equal to zero
      if (  programStatus != PS_OK )
      {
         gLog() << Msg::Error << "Application exiting with code " << programStatus << Msg::EndReq;
      }

      gLog() << Msg::Info << "Finalising..." << Msg::EndReq;

      /// Clear singletons
      delete &SingletonStore::getInstance();

      gLog() << Msg::Info << "Bye." << Msg::EndReq;

      /// Close global logger (special singleton)
      closeGlobalLogger();
      delete &GlobalLogParameters::getInstance();

      /// This is the exit point if ROOT is running
      RootUtilities::closeRootAppIfRunning( programStatus );

      /// Exit
      if ( gApp )
      {
         DBG_MSG( "Deleting the GUI application..." );
         delete gApp;
      }
      DBG_MSG( "Leaving finaliseApplication" );
      DBG_MSG( "Leaving " << GlobalParameters::getProgramName() << "(exit code " << programStatus << ")" );
   }
   catch ( BaseException& exc )
   {
      std::cerr << "Exception raised during finalisation!" << Msg::EndReq;
      std::cerr << exc << std::endl;
      throw ImmediateStopExecutionException();
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Main program
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main( int argc, char* argv[] )
{
   DBG_MSG( "In main..." );
   std::cout << "Running " << GlobalParameters::getProgramName() << "..." << std::endl;
   try
   {
      try
      {
         /// Initialise application and parse program options
         const ProgramOptions* programOptions = initialiseApplication( argc, argv );

         if ( !programOptions )
         {
            programStatus = PS_UNRECOVERABLE_USER_EXCEPTION;
            return programStatus;
         }

         /// Start Qt Application
         if ( programOptions->useQtInterface() )
         {
            gApp = new QApplication( argc, argv );
         }

         /// Run tests
         if ( programOptions->doRunTests() )
         {
            runTests( programOptions );
         }
         if ( programOptions->doRunDevelopmentCode() )
         {
            runDevelopmentCode( programOptions );
         }
         if ( programOptions->getRootFileOutput() != "" )
         {
            saveRootFileOutput( programOptions );
         }
         if ( programOptions->doCompareRootFiles() )
         {
            compareRootFiles( programOptions );
         }

         /// Finalisation
         bool doProcessRootEvents = programOptions->doProcessRootEvents();

         if ( doProcessRootEvents )
         {
            if ( programStatus == PS_OK )
            {
               std::cout << "Running complete." << std::endl;
               std::cout << "Press CTRL+C to end...";
               std::cout.flush();

               RootUtilities::processRootEvents();
            }
         }
         if ( programOptions->useQtInterface() )
         {
            if ( programStatus == PS_OK )
            {
               DevGui::execute();

               Gui::MainWindow mainWindow;
               mainWindow.show();

               if ( gApp->exec() != 0 )
               {
                  programStatus = PS_GUI_NONZERO_STATUSCODE;
                  throw StopExecutionException();
               }
            }
         }
         delete programOptions;
      }
      catch ( const StopExecutionException& )
      {
         gLog() << Msg::Warning << "Trying to shut down gracefully..." << Msg::EndReq;
      }
      finaliseApplication();
      return programStatus;
   }
   catch ( const ImmediateStopExecutionException& )
   {
      DBG_MSG( "ImmediateStopExecutionException caught!" );
      std::cerr << GlobalParameters::getProgramName() << " is shutting down immediately (exitcode = " << programStatus << ")" << std::endl;
      return programStatus;
   }
   catch ( ... )
   {
      DBG_MSG( "Uncaught exception in main!" );
      std::cerr << "Unknown exception occurred!" << std::endl;
      return PS_UNCAUGHT_INTERNAL_EXCEPTION;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Test suite
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void runTests( const ProgramOptions* programOptions )
{
   assert( programOptions );
   assert( programStatus == PS_OK );
   try
   {
      TestSuite::execute();
   }
   catch ( ExceptionTestFailed exc )
   {
      gLog() << Msg::Fatal << "Test failed!" << Msg::EndReq;
      gLog() << Msg::Error << exc << Msg::EndReq;
      programStatus = PS_TEST_FAILED;
      throw StopExecutionException();
   }
   catch ( const BaseException& exc )
   {
      gLog() << Msg::Fatal << "Test failed!" << Msg::EndReq;
      gLog() << Msg::Error << exc << Msg::EndReq;
      programStatus = PS_UNRECOVERABLE_INTERNAL_EXCEPTION;
      throw StopExecutionException();
   }
   gLog() << Msg::Info << "Running tests successful." << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Development area
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void runDevelopmentCode( const ProgramOptions* programOptions )
{
   assert( programOptions );
   try
   {
      DevSuite::execute();
   }
   catch ( const BaseException& exc )
   {
      gLog() << Msg::Fatal << "Test failed!" << Msg::EndReq;
      gLog() << Msg::Error << exc << Msg::EndReq;
      programStatus = PS_UNRECOVERABLE_INTERNAL_EXCEPTION;
      throw StopExecutionException();
   }
   gLog() << Msg::Info << "Running development code done." << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// saveRootFileOutput
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void saveRootFileOutput( const ProgramOptions* programOptions )
{
   assert( programOptions );
   try
   {
      RootUtilities::getInstance().saveAllRootObjectsToFile( programOptions->getRootFileOutput() );
   }
   catch ( const BaseException& exc )
   {
      gLog() << Msg::Fatal << "Error while saving root output file!" << Msg::EndReq;
      gLog() << Msg::Error << exc << Msg::EndReq;
      programStatus = PS_UNRECOVERABLE_INTERNAL_EXCEPTION;
      throw StopExecutionException();
   }
   gLog() << Msg::Info << "Results successfully save to file." << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// compareRootFiles
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void compareRootFiles( const ProgramOptions* programOptions )
{
   assert( programOptions );
   try
   {
      gLog() << Msg::Info << "Comparing root files: " << Msg::EndReq;
      gLog() << Msg::Info << "* Old = " << programOptions->getRootFileNameCompareOld() << Msg::EndReq;
      gLog() << Msg::Info << "* New = " << programOptions->getRootFileNameCompareNew() << Msg::EndReq;
      RootFileCompare comparison( programOptions->getRootFileNameCompareOld(), programOptions->getRootFileNameCompareNew() );
      comparison.compareAll();
   }
   catch ( BaseException& exc )
   {
      gLog() << Msg::Fatal << "Error while comparing ROOT files!" << Msg::EndReq;
      gLog() << Msg::Error << exc << Msg::EndReq;
      programStatus = PS_UNRECOVERABLE_INTERNAL_EXCEPTION;
      throw StopExecutionException();
   }
   gLog() << Msg::Info << "Comparing ROOT-files complete." << Msg::EndReq;
}
