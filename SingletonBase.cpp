#include "SingletonBase.h"

#include "SingletonStore.h"

SingletonBase::SingletonBase( const std::string& className ) :
   className( className )
{
   SingletonStore::getInstance().registerSingleton( this );
}

SingletonBase::~SingletonBase()
{
   SingletonStore::getInstance().unregisterSingleton( this );
}
