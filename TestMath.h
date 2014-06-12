#ifndef TESTMATH_H
#define TESTMATH_H

/**
 * @class TestMath
 * @brief Test suite for mathemathical algorithms and constructs.
 */
class TestMath
{
   public:
      /**
       * Execute all tests.
       */
      static void execute();

   /**
    * Gradient-based optimisation algorithms.
    */
   private:
      static void testTwoDimExampleObjective();
      static void testGradDescOptimiser();

   /**
    * Stochastic optimisation algorithms.
    */
   private:
      static void testParticleSwarm();
      static void testMcmc();

   /**
    * Math containers.
    */
   private:
      static void testSampledMovingAverage();
      static void testTwoTuple();
      static void testRegularAccumArray();

   /**
    * Other/uncategorized.
    */
   private:
      static void testNewtonSolver1D();
      static void testPdf();

};

#endif // TESTMATH_H
