#ifndef ROOTFILECOMPARE_H
#define ROOTFILECOMPARE_H

#include <string>
#include "TString.h"

class TFile;
class TGraph;

class Logger;

typedef std::vector< TString > TStringList;

/**
 * @class RootFileCompare
 * @brief Compares the contents of ROOT files.
 * Comparison of the following ROOT objects are supported:
 * - TGraph
 *
 * Not supported objects:
 * - TGraphAsymmErrors
 */
class RootFileCompare
{
   public:
      /**
       * Constructor/destructor.
       * A call to compareAll is needed in order to actually do something.
       */
      RootFileCompare( const std::string& fileNameOld, const std::string& fileNameNew );
      ~RootFileCompare();

      /**
       * Compare everything that is implemented.
       */
      void compareAll();

   private:
      /**
       * Compare keys.
       */
      TStringList compareKeys();
      /**
       * Check whether the same key points to the same type of object.
       */
      TStringList compareObjectTypes( const TStringList& commonKeys );
      /**
       * Compares all objects by delegating comparison.
       */
      void compareRootObjects( const TStringList& keys );

   /** ==================================================================================
    * Comparison delegates (compare method, as well as visualisation routines in private)
    */

   /**
    * Compare TGraphs
    */
   public:
      bool compareGraphs( TGraph& graphOld, TGraph& graphNew );
   private:
      void createDifferenceGraphsSameNPoints( TGraph& graphOld, TGraph& graphNew );

   /**
    * Access Logger
    */
   private:
      Logger& getLogger() const;

   /**
    * Mutable logger pattern
    */
   private:
      mutable Logger* m_logger;

   /**
    * Private members
    */
   private:
      TFile*            m_fOld;
      TFile*            m_fNew;
      bool              m_haveIdenticalKeys;
      bool              m_haveIdenticalData;


};

#endif // ROOTFILECOMPARE_H
