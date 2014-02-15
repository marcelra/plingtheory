#ifndef GLOBALPARAMETERS_H
#define GLOBALPARAMETERS_H

#include "SingletonBase.h"

#include <string>

class ProgramOptions;

/**
 * @class GlobalParameters
 * @brief Hack class that contains parameters needed for locating directories etc.
 */
class GlobalParameters : SingletonBase
{
   public:
      /**
       * Get the path of the trunk
       */
      static std::string getTrunkPath();

      /**
       * Get run directory
       * This is the directory in which the executable is run
       */
      static std::string getRunDir();

      /**
       * Returns the name of the program
       */
      static const char* getProgramName();

      /**
       * Retreive the directory in which the test data is located
       */
      static const std::string& getTestDataDir();

   private:
      /**
       * Sets the directory in which the test data is located
       */
      static void setTestDataDir( const std::string& dir );

   private:
      static std::string       s_testDataDir;

   /**
    * Friend method
    */
   friend const ProgramOptions* initialiseApplication( int argc, char* argv[] );

};

#endif // GLOBALPARAMETERS_H
