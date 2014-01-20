#ifndef ALGORITHMEXCEPTIONS_H
#define ALGORITHMEXCEPTIONS_H

#include "Exceptions.h"

class AlgorithmBase;

/**
 * @class AlgorithmException
 * @brief Base class for all algorithmic exceptions
 */
class AlgorithmException : public BaseException
{
};

/**
 * @brief AlgorithmCannotExecuteTwice
 * @brief Raised when algorithm is executed more than once
 */
class AlgorithmCannotExecuteTwice : public AlgorithmException
{
   public:
      AlgorithmCannotExecuteTwice( const AlgorithmBase& algorithm );
      const char* getType() const;
};

/**
 * @class AlgorithmTooManyIterations
 * @brief Raised when algorithm requires too many iterations to finish
 */
class AlgorithmTooManyIterations : public AlgorithmException
{
   public:
      AlgorithmTooManyIterations( const std::string& algorithmName, size_t iterationCount );
      const char* getType() const;
};


#endif // ALGORITHMEXCEPTIONS_H
