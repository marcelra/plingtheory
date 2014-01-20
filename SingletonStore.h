#ifndef SINGLETONSTORE_H
#define SINGLETONSTORE_H

#include "Logger.h"

#include <vector>

class SingletonBase;

class SingletonStore
{
   public:
      ~SingletonStore();

      static SingletonStore& getInstance();
      void registerSingleton( SingletonBase* singleton );
      void unregisterSingleton( SingletonBase* singleton );

   private:
      SingletonStore();

      static SingletonStore*        theInstance;
      std::vector< SingletonBase* > singletons;
      Logger                        logger;
};

#endif // SINGLETONSTORE_H
