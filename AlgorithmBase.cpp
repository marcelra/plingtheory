#include "AlgorithmBase.h"

#include "Logger.h"

#include <sstream>

AlgorithmBase::AlgorithmBase( const std::string& name, const AlgorithmBase* parent ) :
   LoggerClient( name ),
   m_parent( parent )
{
   if ( m_parent )
   {
      m_hierarchyCount = m_parent->getHierarchyCount();
      getLogger().setMessagePrefix( formatIndentStr() );
   }
   else
   {
      m_hierarchyCount = 0;
   }
}

AlgorithmBase::~AlgorithmBase()
{
}

size_t AlgorithmBase::getHierarchyCount() const
{
   return m_hierarchyCount;
}

std::string AlgorithmBase::formatIndentStr() const
{
   std::ostringstream result;
   for ( size_t i = 0; i < m_hierarchyCount; ++i )
   {
      result << ".";
   }
   result << "> ";
   return result.str();
}


