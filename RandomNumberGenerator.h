#ifndef RANDOMNUMBERGENERATOR_H
#define RANDOMNUMBERGENERATOR_H

#include <cstddef>

#include "TRandom3.h"


/**
 * @class RandomNumberGenerator
 * @brief Class that generates random numbers. Wraps the ROOT TRandom3 class.
 */
class RandomNumberGenerator
{
   public:
      /**
       * Constructor.
       */
      RandomNumberGenerator( size_t seed );
      /**
       * Destructor.
       */
      virtual ~RandomNumberGenerator();

      /**
       * Generate random numbers between 0 and 1.
       */
      double uniform();
      /**
       * Generate random numbers between @param xMin and @param xMax.
       */
      double uniform( double xMin, double xMax );
      /**
       * Generate normal distributed numbers with mean @param mu and variance @param sigma^2
       */
      double gauss( double mean, double sigma );

   private:
      TRandom3*      m_ranGen;         //! ROOT random number generator.
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods RandomNumberGenerator
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// uniform
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double RandomNumberGenerator::uniform()
{
   return m_ranGen->Uniform();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// uniform
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double RandomNumberGenerator::uniform( double xMin, double xMax )
{
   return m_ranGen->Uniform( xMin, xMax );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// gauss
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double RandomNumberGenerator::gauss( double mean, double sigma )
{
   return m_ranGen->Gaus( mean, sigma );
}


#endif // RANDOMNUMBERGENERATOR_H