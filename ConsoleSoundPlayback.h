#ifndef CONSOLESOUNDPLAYBACK_H
#define CONSOLESOUNDPLAYBACK_H

#include "Logger.h"

class ConsoleSoundPlayback
{
   public:
      static void play( const std::string& fileName );
};

int testConsoleSoundPlayback();

#endif // CONSOLESOUNDPLAYBACK_H
