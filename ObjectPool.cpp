#include "ObjectPool.h"

#include "Exceptions.h"
#include "Logger.h"

#include <iomanip>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ObjectPool::ObjectPool() :
   SingletonBase( "ObjectPool" ),
   m_logger( new Logger( "ObjectPool" ) )
{
   getLogger().setThreshold( Msg::Info );
   getLogger() << Msg::Debug << "Initialising the ObjectPool" << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ObjectPool::~ObjectPool()
{
   getLogger() << Msg::Debug << "Clearing the ObjectPool" << Msg::EndReq;
   for ( std::map< std::string, IStorable* >::iterator it = m_store.begin(); it != m_store.end(); ++it )
   {
      delete it->second;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Access to instance
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ObjectPool& ObjectPool::getInstance()
{
   if ( !s_instance )
   {
      s_instance = new ObjectPool();
   }
   return *s_instance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// deleteItem
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectPool::deleteItem( const std::string& key )
{
   getLogger() << Msg::Verbose << "Object with key <" << key << "> will be deleted." << Msg::EndReq;
   StoreIt it = m_store.find( key );
   if ( it == m_store.end() )
   {
      throw ExceptionKeyNotFound( key, "ObjectPool" );
   }
   delete it->second;
   m_store.erase( it );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// releaseItem
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectPool::releaseItem( const std::string& key )
{
   getLogger() << Msg::Verbose << "Object with key <" << key << "> will be released." << Msg::EndReq;
   StoreIt it = m_store.find( key );
   if ( it == m_store.end() )
   {
      throw ExceptionKeyNotFound( key, "ObjectPool" );
   }
   m_store.erase( it );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// dump
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ObjectPool::dump( std::ostream& os ) const
{
   os << "------------------------------------------------------------------------------------------"  << std::endl;
   os << "Current object pool" << std::endl;
   if ( m_store.begin() == m_store.end() )
   {
      os << "<The object store is empty>" << std::endl;
   }
   else
   {
      size_t keyFieldSize = 40;
      size_t typeFieldSize = 30;

      os << std::left;
      os << "\033[1m";
      os << std::setw( keyFieldSize ) << "Key" << std::setw( typeFieldSize ) << "Type" << "Pointer" << std::endl;
      os << "\033[0m";
   os << "------------------------------------------------------------------------------------------"  << std::endl;
      for ( StoreConstIt it = m_store.begin(); it != m_store.end(); ++it )
      {
         os << std::setw( keyFieldSize ) << it->first << std::setw( typeFieldSize ) << it->second->getType() << it->second << std::endl;
      }
   }
   os << "------------------------------------------------------------------------------------------"  << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Singleton instance
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ObjectPool* ObjectPool::s_instance = 0;


