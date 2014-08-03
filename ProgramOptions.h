#ifndef PROGRAMOPTIONS_H
#define PROGRAMOPTIONS_H

#include "Msg.h"

#include <list>
#include <map>
#include <string>

typedef std::list< std::string > StringList;

class ProgramOptions
{
   public:
      ProgramOptions( int argc, char* argv[] );
      ~ProgramOptions();
      static const ProgramOptions& getInstance();

   public:
      bool hasErrors() const;
      static void printOptions( std::ostream& os );

   /**
    * Actions
    */
   public:
      bool doRunTests() const;
      bool doRunSingleTest() const;
      bool doRunDevelopmentCode() const;
      bool doCompareRootFiles() const;

   /**
    * Options
    */
   public:
      bool                 useRootInterface() const;
      bool                 useQtInterface() const;
      bool                 doUseColorLogger() const;
      bool                 doShowLoggerIds() const;
      bool                 doUseRegressionLogConfig() const;
      const std::string&   getLogFileName() const;
      const std::string&   getDataDir() const;
      const std::string&   getRootFileNameCompareOld() const;
      const std::string&   getRootFileNameCompareNew() const;
      const std::string&   getRootFileOutput() const;
      int                  getLogLevel() const;

      const std::map< size_t, Msg::LogLevel >& getLoggerInspectMap() const;

   private:
      void parseArguments();
      StringList::const_iterator safeAdvanceIter( StringList::const_iterator, const StringList& list, const std::string& currentOption ) const;

   private:
      static ProgramOptions*   s_instance;
      StringList               m_argList;
      bool                     m_hasErrors;

   private:
      bool              m_doRunTests;
      bool					m_doRunSingleTest;
      bool              m_doRunDevelopmentCode;
      bool              m_doCompareRootFiles;

   private:
      bool                    m_useRootInterface;
      bool                    m_useQtInterface;
      bool                    m_doUseColorLogger;
      bool                    m_doShowLoggerIds;
      bool                    m_useRegressionLogConfig;
      std::string             m_rootFileNameOld;
      std::string             m_rootFileNameNew;
      std::string             m_logFileName;
      std::string             m_dataDir;
      std::string             m_rootFileOutput;
      int                     m_logLevel;

      std::map< size_t, Msg::LogLevel > m_inspectLogIds;

};

#endif // PROGRAMOPTIONS_H
