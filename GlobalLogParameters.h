#ifndef GLOBALLOGPARAMETERS_H
#define GLOBALLOGPARAMETERS_H

#include "Msg.h"

/**
 * @class GlobalLogParameters
 * @brief Singleton class that holds the global logging parameters
 */
class GlobalLogParameters
{
   public:
      /**
       * Get the singleton instance
       */
      static const GlobalLogParameters& getInstance();

      /**
       * Set global threshold
       */
      void setThreshold( Msg::LogLevel threshold );

      /**
       * Get global threshold
       */
      const Msg::LogLevel& getThreshold() const;
      /**
       * Get the field width in which the name is displayed
       */
      size_t getNameFieldWidth() const;
      /**
       * Get the field width in which the level is displayed
       */
      size_t getLevelFieldWidth() const;
      /**
       * Get the number of spaces between the fields
       */
      size_t getSpacerWidth() const;

   private:
      /**
       * Constructor
       */
      GlobalLogParameters();

      static GlobalLogParameters*    theInstance;     //! Singleton instance

      size_t         nameFieldWidth;      //! Width of the name field
      size_t         levelFieldWidth;     //! Width of the level field
      size_t         spacerWidth;         //! Number of spaces between fields
      Msg::LogLevel  threshold;           //! Global threshold
};

#endif // GLOBALLOGPARAMETERS_H
