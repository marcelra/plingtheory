#include "AlgorithmBase.h"

#include "AlgorithmExceptions.h"
#include "ObjectPool.h"

AlgorithmBase::AlgorithmBase( const std::string& typeName, const std::string& instanceName, bool allowMultipleExecute ) :
   m_logger( instanceName ),
   m_typeName( typeName ),
   m_instanceName( instanceName ),
   m_allowMultipleExecute( allowMultipleExecute ),
   m_hasExecuted( false )
{
   getObjectPool().store( instanceName, this );
}

AlgorithmBase::~AlgorithmBase()
{
   getObjectPool().releaseItem( m_instanceName );
}

void AlgorithmBase::execute()
{
   if ( !m_hasExecuted || m_allowMultipleExecute )
   {
      executeImpl();
      m_hasExecuted = true;
   }
   else
   {
      throw AlgorithmCannotExecuteTwice( *this );
   }
}

void AlgorithmBase::executeImpl()
{
   getLogger() << Msg::Info << "In executeImpl" << Msg::EndReq;
}

ObjectPool& AlgorithmBase::getObjectPool()
{
   return ObjectPool::getInstance();
}

const ObjectPool& AlgorithmBase::getObjectPool() const
{
   return ObjectPool::getInstance();
}
