#ifndef ROOTUTILITIES_H
#define ROOTUTILITIES_H

#include "SingletonBase.h"
#include "Utils.h"

/// ROOT
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TRandom3.h"
#include "TString.h"

/// STL
#include <cassert>
#include <map>

/// Forward declares
class RawPcmData;
class TApplication;
class Logger;

class RootUtilities : public SingletonBase
{
   public:
      /**
       * Destructor
       */
      ~RootUtilities();

      /**
       * Initialise the ROOT gui
       * This make it possible to show plots
       */
      static void initRootApplication();
      /**
       * Enter the ROOT event loop
       */
      static void processRootEvents();
      /**
       * Close the ROOT application if it is running
       * This method can be called without knowledge if ROOT is running
       */
      static void closeRootAppIfRunning( int exitCode );
      /**
       * Access singleton object
       */
      static RootUtilities& getInstance();

   /**
    * Plotting methods
    */
   public:
      /**
       * Create a graph (all data points are equidistant)
       */
      template < class T >
      static TGraph* createGraph( const std::vector< T >& yData );
      /**
       * Create a graph
       */
      template < class T >
      static TGraph* createGraph( const std::vector< T >& xData, const std::vector< T >& yData );
      /**
       * Create a graph with error bars
       */
      template < class T >
      static TGraphErrors* createGraphErrors( const std::vector< T >& xData, const std::vector< T >& yData,
                                        const std::vector< T >& xErrors, const std::vector< T >& yErrors );
      /**
       * Create a graph from RawPcmData
       */
      static TGraph* createGraph( const RawPcmData& data );
      /**
       * Create histogram
       * The data determines the histogram x-axis range
       */
      template < class T >
      static TH1F* createHist( const std::vector< T >& data, size_t nBins, const std::string& name = "" );

      /**
       * Set default style to graph
       */
      static void dressGraph( TGraph* graph );

      /**
       * Create a decent legend
       */
      static TLegend* createDefaultLegend( double x1 = 0.5, double y1 = 0.7, double x2 = 0.7, double y2 = 0.9 );

      /**
       * Generates a unique name
       * Name will have the following formatting: <baseName>_0
       * When no basename is given, it will be set to "auto"
       */
      TString generateUniqueName( const std::string& baseName );

      /**
       * Save all root objects to root file
       */
      void saveAllRootObjectsToFile( const std::string& fileName );

   private:
      /**
       * Access logger
       */
      Logger& getLogger();

      /**
       * Register a new graph
       */
      void registerGraph( TGraph* graph );

      /**
       * Constructor
       */
      RootUtilities();

   private:
      static RootUtilities*       m_theInstance;    //! Singleton instance
      Logger*                     m_logger;         //! Logger
      std::map< TString, size_t > m_uniqueNameIds;  //! Basename vs. Id
      std::vector< TGraph* >      m_graphList;      //! List with graphs
};



////////////////////////////////////////////////////////////////////////////////
/// template implementations
////////////////////////////////////////////////////////////////////////////////
template < class T >
TGraph* RootUtilities::createGraph( const std::vector< T >& yData )
{
   std::vector< double > xData( yData.size() );
   for ( size_t i = 0; i < yData.size(); ++i )
   {
      xData[i] = i;
   }
   return createGraph( xData, yData );
}

template < class T >
TGraph* RootUtilities::createGraph( const std::vector< T >& xData, const std::vector< T >& yData )
{
   assert( xData.size() == yData.size() );
   size_t nPoints = xData.size();

   double* xArr = new double[ nPoints ];
   double* yArr = new double[ nPoints ];

   for ( size_t i = 0; i < nPoints; ++i )
   {
      xArr[i] = static_cast< double >( xData[i] );
      yArr[i] = static_cast< double >( yData[i] );
   }

   TGraph* result = new TGraph( nPoints, xArr, yArr );
   dressGraph( result );
   RootUtilities::getInstance().registerGraph( result );
   delete[] xArr;
   delete[] yArr;
   return result;
}

template < class T >
TGraphErrors* RootUtilities::createGraphErrors( const std::vector< T >& xData, const std::vector< T >& yData,
                                                const std::vector< T >& xErrors, const std::vector< T >& yErrors )
{
   assert( xData.size() == yData.size() );
   assert( xErrors.size() == yErrors.size() );
   assert( xData.size() == xErrors.size() );

   size_t nPoints = xData.size();
   double* xArr = new double [ nPoints ];
   double* yArr = new double [ nPoints ];
   double* xErr = new double [ nPoints ];
   double* yErr = new double [ nPoints ];

   for ( size_t i = 0; i < nPoints; ++i )
   {
      xArr[i] = static_cast< double >( xData[i] );
      yArr[i] = static_cast< double >( yData[i] );
      xErr[i] = static_cast< double >( xErrors[i] );
      yErr[i] = static_cast< double >( yErrors[i] );
   }

   TGraphErrors* result = new TGraphErrors( nPoints, xArr, yArr, xErr, yErr );
   dressGraph( result );
   RootUtilities::getInstance().registerGraph( result );
   delete[] xArr;
   delete[] yArr;
   delete[] xErr;
   delete[] yErr;
   return result;
}

template < class T >
TH1F* RootUtilities::createHist( const std::vector< T >& data, size_t nBins, const std::string& name )
{
   RootUtilities& rootUtils = RootUtilities::getInstance();
   TString histName = rootUtils.generateUniqueName( name );
   double xMin = Utils::getMinValue( data );
   double xMax = Utils::getMaxValue( data );

   TH1F* hist = new TH1F( histName, histName, nBins, xMin, xMax );
   for ( size_t i = 0; i < data.size(); ++i )
   {
      hist->Fill( data[i] );
   }
   return hist;
}


////////////////////////////////////////////////////////////////////////////////
/// test method
////////////////////////////////////////////////////////////////////////////////
int testRootUtilities();

#endif // ROOTUTILITIES_H
