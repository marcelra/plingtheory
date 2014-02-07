#include "RootUtilities.h"

#include "Exceptions.h"
#include "GlobalParameters.h"
#include "RawPcmData.h"

#include "TApplication.h"
#include "TROOT.h"
#include "TStyle.h"

////////////////////////////////////////////////////////////////////////////////
/// initRootApplication
////////////////////////////////////////////////////////////////////////////////
void RootUtilities::initRootApplication()
{
   /// Check if gApplication is still unitialised
   if ( !gApplication )
   {
      RootUtilities& ru = RootUtilities::getInstance();
      ru.getLogger() << Msg::Debug << "Starting ROOT application" << Msg::EndReq;

      /// Initialise root application
      /// This will set gApplication
      int argc = 0;
      char** argv = 0;
      new TApplication( "ROOTApplication", &argc, argv );

      /// Set decent root style
      std::string rootLogonScript = GlobalParameters::getRunDir() + "/" + "rootlogon.C";
      gApplication->ProcessFile( rootLogonScript.c_str() );

      ru.getLogger() << Msg::Debug << "ROOT Application initialised." << Msg::EndReq;
   }
   else
   {
      /// Programming error
      assert( false && "initRootApplication should be called only once" );
   }
}

////////////////////////////////////////////////////////////////////////////////
/// processRootEvents
////////////////////////////////////////////////////////////////////////////////
void RootUtilities::processRootEvents()
{
   if ( gApplication )
   {
      gApplication->Run();
   }
}

////////////////////////////////////////////////////////////////////////////////
/// closeRootAppIfRunning
////////////////////////////////////////////////////////////////////////////////
void RootUtilities::closeRootAppIfRunning( int exitCode )
{
   if ( gApplication )
   {
      gApplication->Terminate( exitCode );
      /// Root will exit the main application
      assert( false );
   }
}

////////////////////////////////////////////////////////////////////////////////
/// getLogger
////////////////////////////////////////////////////////////////////////////////
Logger& RootUtilities::getLogger()
{
   return *m_logger;
}

////////////////////////////////////////////////////////////////////////////////
/// Singleton access
////////////////////////////////////////////////////////////////////////////////
RootUtilities& RootUtilities::getInstance()
{
   if ( !m_theInstance )
   {
      m_theInstance = new RootUtilities();
   }
   return *m_theInstance;
}

////////////////////////////////////////////////////////////////////////////////
/// Constructor
////////////////////////////////////////////////////////////////////////////////
RootUtilities::RootUtilities() :
   SingletonBase( "RootUtilities" ),
   m_logger( new Logger( "RootUtilities" ) )
{
   getLogger() << Msg::Debug << "RootUtilities initialised." << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////////////////////////
RootUtilities::~RootUtilities()
{
   getLogger() << Msg::Debug << "RootUtilities destructor" << Msg::EndReq;
   delete m_logger;
}

////////////////////////////////////////////////////////////////////////////////
/// static members
////////////////////////////////////////////////////////////////////////////////
RootUtilities* RootUtilities::m_theInstance = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// createGraph
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TGraph* RootUtilities::createGraph( const RawPcmData& rawPcmData )
{
   RealVectorPtr data = rawPcmData.copyToVectorData();
   TGraph* result = createGraph( *data );
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// dressGraph
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RootUtilities::dressGraph( TGraph* graph )
{
   graph->SetLineWidth( 2 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// createDefaultLegend
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TLegend* RootUtilities::createDefaultLegend( double x1, double y1, double x2, double y2 )
{
   TLegend* result = new TLegend( x1, y1, x2, y2 );
   result->SetBorderSize( 0 );
   result->SetFillColor( 0 );
   result->SetTextFont( gStyle->GetTextFont() );
   result->SetTextSize( gStyle->GetTextSize() );
   return result;
}


TString RootUtilities::generateUniqueName( const std::string& baseName )
{
   TString tBaseName = baseName;
   if ( baseName.size() == 0 )
   {
      tBaseName = "auto";
   }

   std::map< TString, size_t >::iterator it = m_uniqueNameIds.find( tBaseName );
   size_t maxId = 0;
   if ( it == m_uniqueNameIds.end() )
   {
      maxId = 0;
      while ( true )
      {
         TString freeNamePoll = tBaseName;
         freeNamePoll += "_";
         freeNamePoll += maxId;
         if ( !gROOT->FindObject( freeNamePoll ) )
         {
            break;
         }
         else
         {
            ++maxId;
         }
      }
      m_uniqueNameIds.insert( std::pair< TString, size_t >( tBaseName, maxId ) );
   }
   else
   {
      maxId = it->second;
      ++maxId;
      while ( true )
      {
         TString freeNamePoll = tBaseName;
         freeNamePoll += "_";
         freeNamePoll += maxId;
         if ( !gROOT->FindObject( freeNamePoll ) )
         {
            break;
         }
         else
         {
            ++maxId;
         }
      }
      it->second = maxId;
   }
   tBaseName += "_";
   tBaseName += maxId;
   return tBaseName;
}

