#ifndef PROGRAMOPTIONS_H
#define PROGRAMOPTIONS_H

#include <string>

/**
 * @class ProgramOptions
 * @brief Dedicated parser for command-line arguments
 */
class ProgramOptions
{
   public:
      ProgramOptions( int argc, char* argv[] );

      /**
       * Returns true if there are unrecognised options given by the user
       */
      bool hasErrors() const;

   /**
    * Parsed option access
    */
   public:
      bool doProcessRootEvents() const;
      bool doRunAllTests() const;
      bool doRunDevelopmentCode() const;

   private:
      bool parseOption( const std::string& option );

   private:
      bool           m_hasErrors;
      bool           m_doProcessRootEvents;
      bool           m_doRunAllTests;
      bool           m_doRunDevelopmentCode;

};

#endif // PROGRAMOPTIONS_H
