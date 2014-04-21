#ifndef RANDOMNUMBERGENERATOR_H
#define RANDOMNUMBERGENERATOR_H

#include <cstddef>

/// Forward declares.
class TRandom3;

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

   private:
      TRandom3*      m_ranGen;         //! ROOT random number generator.
};

#endif // RANDOMNUMBERGENERATOR_H
