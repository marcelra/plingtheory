#ifndef SINGLETON_H
#define SINGLETON_H

#include <string>

class SingletonBase
{
   public:
      virtual ~SingletonBase();

   protected:
      SingletonBase( const std::string& className );

   /**
    * Blocked copy-constructor and assignment operator
    */
   private:
      SingletonBase( const SingletonBase& other );
      SingletonBase& operator=( const SingletonBase& other );

   private:
      std::string    className;
};

#endif // SINGLETON_H
