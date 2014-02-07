#ifndef GLOBALLOGPARAMETERS_H
#define GLOBALLOGPARAMETERS_H

#include "Msg.h"
#include "SingletonBase.h"

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

      ~GlobalLogParameters();

      /**
       * Set global threshold
       */
      void setThreshold( Msg::LogLevel m_threshold );

      /**
       * Set use colors
       */
      void setUseColors( bool useColors );
      /**
       * Open file stream
       */
      void openFileStream( const std::string& fileName );

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
      /**
       * Get color configuration
       */
      bool getUseColors() const;
      /**
       * Gets the ostream to write to
       */
      std::ostream& getStream() const;

   private:
      /**
       * Constructor
       */
      GlobalLogParameters();

      static GlobalLogParameters*    s_theInstance;     //! Singleton instance

      size_t                m_nameFieldWidth;      //! Width of the name field
      size_t                m_levelFieldWidth;     //! Width of the level field
      size_t                m_spacerWidth;         //! Number of spaces between fields
      Msg::LogLevel         m_threshold;           //! Global threshold
      bool                  m_useColors;           //! Flag whether or not to display colors
      mutable std::ostream* m_stream;
      mutable std::fstream* m_fstream;
};

#endif // GLOBALLOGPARAMETERS_H
