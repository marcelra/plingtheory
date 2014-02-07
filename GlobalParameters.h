#ifndef GLOBALPARAMETERS_H
#define GLOBALPARAMETERS_H

#include "SingletonBase.h"

#include <string>

/**
 * @class GlobalParameters
 * @brief Hack class that contains parameters needed for locating directories etc.
 */
class GlobalParameters : SingletonBase
{
   public:
      const GlobalParameters& getInstance();
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

   private:
      GlobalParameters();

   private:
      static GlobalParameters*    s_instance;

};

#endif // GLOBALPARAMETERS_H
