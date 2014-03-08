#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include "Exceptions.h"
#include "Logger.h"
#include "IStorable.h"
#include "SingletonBase.h"

#include <map>

class Logger;

/**
 * @class ObjectPool
 * @brief Storage of IStorable objects so that these can be exchanged between algorithms
 */
class ObjectPool : SingletonBase
{
   public:
      /**
       * Get singleton instance
       */
      static ObjectPool& getInstance();

      /**
       * Delete instance
       */
      virtual ~ObjectPool();

      /**
       * Get access to object stored with key @param key.
       * The result is stored in pointer @param ptr.
       */
      template <class T>
      void retrieve( const std::string& key, const T*& ptr ) const;

      /**
       * Display contents to std::ostream @param os
       */
      void dump( std::ostream& os ) const;

      /**
       * Store an object @param object with key @param key. The ObjectPool takes ownership of the object.
       */
      template <class T>
      void store( const std::string& key, T* object );
      /**
       * Remove object indexed with key @param key.
       */
      void deleteItem( const std::string& key );
      /**
       * Release ownership of object indexed with key @param key
       */
      void releaseItem( const std::string& key );

      Logger& getLogger() const;

   private:
      /**
       * Private constructor (singleton pattern)
       */
      ObjectPool();

      typedef std::map< std::string, IStorable* > Store;
      typedef Store::iterator                     StoreIt;
      typedef Store::const_iterator               StoreConstIt;

      Store           m_store;
      mutable Logger* m_logger;

      static ObjectPool*      s_instance;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Template method ObjectPool::retrieve
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
void ObjectPool::retrieve( const std::string& key, const T*& ptr ) const
{
   if ( ptr )
   {
      getLogger() << Msg::Warning << "Parameter ptr is not equal to zero. An existing object might be overwritten." << Msg::EndReq;
   }
   getLogger() << Msg::Verbose << "Trying to obtain object with key <" << key << "> from object store." << Msg::EndReq;
   StoreConstIt it = m_store.find( key );
   if ( it == m_store.end() )
   {
      throw ExceptionKeyNotFound( key, "ObjectPool" );
   }
   getLogger() << Msg::Verbose << "Found object at " << it->second << Msg::EndReq;
   ptr = dynamic_cast<T*>( it->second );
   if ( !ptr )
   {
      throw ExceptionTypeConflict( "ObjectPool" );
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Template method ObjectPool::retrieve
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
void ObjectPool::store( const std::string& key, T* object )
{
   getLogger() << Msg::Verbose << "Storing object " << object << " with key <" << key << "> in the object store." << Msg::EndReq;
   m_store[key] = object;
}

inline Logger& ObjectPool::getLogger() const
{
   return *m_logger;
}

#endif // OBJECTPOOL_H

