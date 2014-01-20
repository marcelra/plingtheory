#include "AlgorithmExceptions.h"

#include "AlgorithmBase.h"

#include <sstream>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AlgorithmCannotExecuteTwice
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AlgorithmCannotExecuteTwice::AlgorithmCannotExecuteTwice( const AlgorithmBase& alg )
{
   std::ostringstream msg;
   msg << "Algorithm of type " << alg.getTypeName() << ", named " << alg.getInstanceName() << ", cannot be executed more than once.";
   setMessage( msg.str() );
}

const char* AlgorithmCannotExecuteTwice::getType() const
{
   return "AlgorithmTooManyIterations";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AlgorihtmTooManyIterations
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AlgorithmTooManyIterations::AlgorithmTooManyIterations( const std::string& algorithmName, size_t iterationCount )
{
   std::ostringstream msg;
   msg << algorithmName << " failed to converge after " << iterationCount << " iterations";
   setMessage( msg.str() );
}

const char* AlgorithmTooManyIterations::getType() const
{
   return "AlgorithmTooManyIterations";
}
