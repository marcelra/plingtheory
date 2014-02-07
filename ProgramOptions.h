#ifndef PROGRAMOPTIONS_H
#define PROGRAMOPTIONS_H

#include <string>
#include <list>

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
      bool doRunDevelopmentCode() const;

   /**
    * Options
    */
   public:
      bool                 doProcessRootEvents() const;
      bool                 doUseColorLogger() const;
      const std::string&   getLogFileName() const;
      const std::string&   getDataDir() const;
      int                  getLogLevel() const;

   private:
      void parseArguments();

   private:
      static ProgramOptions*   s_instance;
      StringList               m_argList;
      bool                     m_hasErrors;

   private:
      bool              m_doRunTests;
      bool              m_doRunDevelopmentCode;

   private:
      bool              m_doProcessRootEvents;
      bool              m_doUseColorLogger;
      std::string       m_logFileName;
      std::string       m_dataDir;
      int               m_logLevel;
};

#endif // PROGRAMOPTIONS_H
