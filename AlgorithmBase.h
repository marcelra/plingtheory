#ifndef ALGORITHMBASE_H
#define ALGORITHMBASE_H

#include "LoggerClient.h"

class AlgorithmBase : public LoggerClient
{
   public:
      AlgorithmBase( const std::string& name, const AlgorithmBase* parent );
      virtual ~AlgorithmBase();

   protected:
      size_t getHierarchyCount() const;

   private:
      std::string formatIndentStr() const;

      const AlgorithmBase* m_parent;

      size_t        m_hierarchyCount;
};

#endif // ALGORITHMBASE_H
