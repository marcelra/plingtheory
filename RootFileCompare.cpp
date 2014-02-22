#include "RootFileCompare.h"

#include "Exceptions.h"
#include "Logger.h"
#include "RootUtilities.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TGraph.h"
#include "TKey.h"
#include "TObject.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RootFileCompare::RootFileCompare( const std::string& fileNameOld, const std::string& fileNameNew ) :
   m_logger( new Logger( "RootFileCompare" ) ),
   m_fOld( new TFile( fileNameOld.c_str() ) ),
   m_fNew( new TFile( fileNameNew.c_str() ) ),
   m_haveIdenticalKeys( true ),
   m_haveIdenticalData( true )
{
   if ( !m_fOld->IsOpen() )
   {
      throw ExceptionFileCannotOpen( fileNameOld );
   }
   if ( !m_fNew->IsOpen() )
   {
      throw ExceptionFileCannotOpen( fileNameNew );
   }

   getLogger() << Msg::Verbose << "Successfully opened root files '" << fileNameOld << "' and '" << fileNameNew << "'" << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RootFileCompare::~RootFileCompare()
{
   delete m_fOld;
   delete m_fNew;

   delete m_logger;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getLogger
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Logger& RootFileCompare::getLogger() const
{
   return *m_logger;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// compareAll
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RootFileCompare::compareAll()
{
   const TStringList& commonKeys = compareKeys();
   const TStringList& comparableObjectKeys = compareObjectTypes( commonKeys );
   compareRootObjects( comparableObjectKeys );

   if ( m_haveIdenticalData && m_haveIdenticalKeys )
   {
      getLogger() << Msg::Info << "Both root files are identical." << Msg::EndReq;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// compareKeys
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TStringList RootFileCompare::compareKeys()
{
   /// Find the keys that are present in the old file
   TStringList oldKeys;
   TList* oldKeysList = m_fOld->GetListOfKeys();
   for ( int iKey = 0; iKey < oldKeysList->GetSize(); ++iKey )
   {
      TKey* keyPtr = dynamic_cast< TKey* >( oldKeysList->At( iKey ) );
      assert( keyPtr );
      getLogger() << Msg::Verbose << "Found Key(old): " << keyPtr->GetName() << Msg::EndReq;
      oldKeys.push_back( keyPtr->GetName() );
   }

   /// Keys that are present in the new rile
   TStringList newKeys;
   /// Keys that are present in both files
   TStringList commonKeys;
   /// Keys that were added w.r.t. the old root file
   TStringList addedKeys;

   /// Find new keys and compare with old keys
   TList* newKeysList = m_fNew->GetListOfKeys();
   for ( int iKey = 0; iKey < newKeysList->GetSize(); ++iKey )
   {
      TKey* keyPtr = dynamic_cast< TKey* >( newKeysList->At( iKey ) );
      assert( keyPtr );
      getLogger() << Msg::Verbose << "Found Key(new): " << keyPtr->GetName() << Msg::EndReq;
      newKeys.push_back( keyPtr->GetName() );

      bool keyFound = false;
      for ( size_t iKeyOld = 0; iKeyOld < oldKeys.size(); ++iKeyOld )
      {
         if ( TString( keyPtr->GetName() ) == oldKeys[ iKeyOld ] )
         {
            commonKeys.push_back( keyPtr->GetName() );
            keyFound = true;
            break;
         }
      }
      if ( !keyFound )
      {
         addedKeys.push_back( keyPtr->GetName() );
         m_haveIdenticalKeys = false;
      }
   }

   /// Report common keys results
   if ( m_haveIdenticalKeys )
   {
      getLogger() << Msg::Info << "Keys of root files are identical." << Msg::EndReq;
      assert( commonKeys.size() == oldKeys.size() );
      assert( commonKeys.size() == newKeys.size() );
   }
   else
   {
      /// Find which keys have been removed
      TStringList removedKeys;
      for ( size_t iOldKey = 0; iOldKey < oldKeys.size(); ++iOldKey )
      {
         bool keyFound = false;
         for ( size_t iCommonKey = 0; iCommonKey < commonKeys.size(); ++iCommonKey )
         {
            if ( oldKeys[ iOldKey ] == commonKeys[ iCommonKey ] )
            {
               keyFound = true;
            }
         }
         if ( !keyFound )
         {
            removedKeys.push_back( oldKeys[ iOldKey ] );
         }
      }

      /// Report keys that have been added
      if ( addedKeys.size() > 0 )
      {
         getLogger() << Msg::Warning << "The following keys are new:" << Msg::EndReq;
         for ( size_t i = 0; i < addedKeys.size(); ++i )
         {
            getLogger() << Msg::Warning << " + " << addedKeys[ i ] << Msg::EndReq;
         }
      }

      /// Report keys that are removed
      if ( removedKeys.size() > 0 )
      {
         getLogger() << Msg::Warning << "The following keys are removed:" << Msg::EndReq;
         for ( size_t i = 0; i < removedKeys.size(); ++i )
         {
            getLogger() << Msg::Warning << " - " << removedKeys[ i ] << Msg::EndReq;
         }
      }
   }

   return commonKeys;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// compareObjects
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TStringList RootFileCompare::compareObjectTypes( const TStringList& commonObjects )
{
   TStringList commonObjectTypes;
   for ( size_t iObject = 0; iObject < commonObjects.size(); ++iObject )
   {
      TObject* objOld = m_fOld->Get( commonObjects[ iObject ] );
      TObject* objNew = m_fNew->Get( commonObjects[ iObject ] );
      assert( objOld && objNew );
      if ( objNew->ClassName() != objOld->ClassName() )
      {
         getLogger() << Msg::Warning << "Object: " << commonObjects[ iObject ] << " has changed type: " << objOld->ClassName() << " (old), " << objNew->ClassName() << " (new)." << Msg::EndReq;
      }
      commonObjectTypes.push_back( commonObjects[ iObject ] );
   }
   return commonObjectTypes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// compareRootObjects
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RootFileCompare::compareRootObjects( const TStringList& keys )
{
   for ( size_t iKey = 0; iKey < keys.size(); ++iKey )
   {
      TObject* objOld = m_fOld->Get( keys[ iKey ] );
      TObject* objNew = m_fNew->Get( keys[ iKey ] );

      if ( dynamic_cast< TGraph* >( objOld ) )
      {
         m_haveIdenticalData &= compareGraphs( dynamic_cast< TGraph& >( *objOld ), dynamic_cast< TGraph& >( *objNew ) );
      }
      else
      {
         getLogger() << Msg::Warning << "No implementation available for comparing ROOT objects of type: " << objOld->ClassName() << Msg::EndReq;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// compareGraphs
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool RootFileCompare::compareGraphs( TGraph& graphOld, TGraph& graphNew )
{
   getLogger() << Msg::Info << "Comparing graphs with key: " << graphOld.GetName() << Msg::EndReq;
   bool identical = true;

   /// Check number of points of graphs
   if ( graphOld.GetN() != graphNew.GetN() )
   {
      getLogger() << Msg::Warning << "Number of points are different between both graphs." << Msg::EndReq;

      /// If number of points are different, show both graphs on different canvasses
      TCanvas* oldGraph = new TCanvas();
      oldGraph->SetTitle( TString( "OLD graph " ) + graphOld.GetName() );
      graphOld.Draw( "AL" );
      TCanvas* newGraph = new TCanvas();
      newGraph->SetTitle( TString( "NEW graph " ) + graphNew.GetName() );
      graphNew.Draw( "AL" );

      /// Report compare graphs has failed
      return false;
   }

   /// Compare individual points
   if ( identical )
   {
      for ( int iPoint = 0; iPoint < graphOld.GetN(); ++iPoint )
      {
         double oldX, oldY;
         double newX, newY;
         graphOld.GetPoint( iPoint, oldX, oldY );
         graphNew.GetPoint( iPoint, newX, newY );
         if ( oldX == newX && oldY == newY ) {}
         else
         {
            identical = false;
            getLogger() << Msg::Verbose << "point " << iPoint << " is different." << Msg::EndReq;
            getLogger() << Msg::Verbose << "(x, y)_old = (" << oldX << ", " << oldY << ")" << Msg::EndReq;
            getLogger() << Msg::Verbose << "(x, y)_new = (" << newX << ", " << newY << ")" << Msg::EndReq;
            getLogger() << Msg::Verbose << "delta (x,y) = (" << newX-oldX << ", " << newY-oldY << ")"  << Msg::EndReq;
         }
      }
   }

   /// Show differences between graphs
   if ( !identical )
   {
      getLogger() << Msg::Warning << "Graphs are not identical!" << Msg::EndReq;
      createDifferenceGraphsSameNPoints( graphOld, graphNew );
      return false;
   }

   assert( identical );
   getLogger() << Msg::Info << "Graphs are identical." << Msg::EndReq;
   return identical;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// createDifferenceGraphSameNPoints
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RootFileCompare::createDifferenceGraphsSameNPoints( TGraph& graphOld, TGraph& graphNew )
{
   assert( graphOld.GetN() == graphNew.GetN() );

   size_t nPoints = graphOld.GetN();
   RealVector xOld( nPoints );
   RealVector xNew( nPoints );
   RealVector yOld( nPoints );
   RealVector yNew( nPoints );

   for ( size_t iPoint = 0; iPoint < nPoints; ++iPoint )
   {
      graphOld.GetPoint( iPoint, xOld[ iPoint ], yOld[ iPoint ] );
      graphNew.GetPoint( iPoint, xNew[ iPoint ], yNew[ iPoint ] );
   }

   TString canTitle;
   canTitle = TString( "xOld vs xNew of graph " ) + graphOld.GetName();
   TCanvas* xCanvas = new TCanvas();
   xCanvas->SetTitle( canTitle );
   TGraph* grXCompare = RootUtilities::createGraph( xOld, xNew );
   grXCompare->Draw( "AP" );

   canTitle = TString( "yOld vs yNew of graph " ) + graphOld.GetName();
   TCanvas* yCanvas = new TCanvas();
   yCanvas->SetTitle( canTitle );
   TGraph* grYCompare = RootUtilities::createGraph( yOld, yNew );
   grYCompare->Draw( "AP" );

   canTitle = TString( "Direct comparison of graph " ) + graphOld.GetName();
   TCanvas* sameCanvas = new TCanvas();
   sameCanvas->SetTitle( canTitle );
   graphOld.Draw( "AP" );
   graphOld.SetMarkerStyle( 5 );
   graphNew.Draw( "PSAME" );
   graphNew.SetMarkerStyle( 2 );
}

