#include "SingletonStore.h"

#include "Logger.h"
#include "SingletonBase.h"

#include <algorithm>

SingletonStore& SingletonStore::getInstance()
{
   if ( !theInstance )
   {
      theInstance = new SingletonStore();
   }
   return *theInstance;
}

void SingletonStore::registerSingleton( SingletonBase* singleton )
{
   singletons.push_back( singleton );
}

void SingletonStore::unregisterSingleton( SingletonBase* singleton )
{
   std::vector< SingletonBase* >::iterator it = std::find( singletons.begin(), singletons.end(), singleton );
   singletons.erase( it );
}

SingletonStore* SingletonStore::theInstance = 0;

SingletonStore::SingletonStore() :
   logger( new Logger( "SingletonStore" ) )
{
   *logger << Msg::Debug << "Initialised" << Msg::EndReq;
}

SingletonStore::~SingletonStore()
{
   *logger << Msg::Debug << "Finalising..." << Msg::EndReq;
   *logger << Msg::Verbose << "Cleaning up " << singletons.size() << " singletons" << Msg::EndReq;

   /// Since singletons unregister themselver, we have to take care in looping over the list
   while ( singletons.size() > 0 )
   {
      delete singletons.back();
   }
   *logger << Msg::Debug << "SingletonStore down" << Msg::EndReq;

   delete logger;
}

