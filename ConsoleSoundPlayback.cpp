#include "ConsoleSoundPlayback.h"

#include "Exceptions.h"

// #include <QSound>

void ConsoleSoundPlayback::play( const std::string& fileName )
{
   assert( false );
   Logger msg( "ConsoleSoundPlayback" );
   if ( fileName == "" )
   {
      msg << Msg::Info << "Not playing back empty file..." << Msg::EndReq;
      return;
   }

   try
   {
      // if ( !QSound::isAvailable() )
      // {
      //    msg << Msg::Error << "No sound available!" << Msg::EndReq;
      //    throw ExceptionNoSound();
      // }
      // QSound qSoundPlayer( fileName.c_str() );

      // qSoundPlayer.play();

      // while ( !qSoundPlayer.isFinished() )
      // {
      //    usleep( 1000 );
      // }
   }
   catch ( ... )
   {
      throw ExceptionNoSound();
   }

}

////////////////////////////////////////////////////////////////////////////////
/// testConsoleSoundPlayback
////////////////////////////////////////////////////////////////////////////////
int testConsoleSoundPlayback()
{
   Logger msg( "testConsoleSoundPlayback" );
   msg << Msg::Info << "In testConsoleSoundPlayback" << Msg::EndReq;

   try
   {
     const std::string fileName = "/Users/marcelra/Dev/Suite_soundArchive/Cscale.wav";
     ConsoleSoundPlayback::play( fileName );
   }
   catch ( ExceptionNoSound exc )
   {
      msg << Msg::Error << exc << Msg::EndReq;
      return 1;
   }
   msg << Msg::Info << "Completed testConsoleSoundPlayback" << Msg::EndReq;
   return 0;
}



