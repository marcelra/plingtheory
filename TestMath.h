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
   public:
      static void testTwoDimExampleObjective();
      static void testGradDescOptimiser();

   /**
    * Stochastic optimisation algorithms.
    */
   public:
      static void testParticleSwarm();
      static void testMcmc();

   /**
    * Math containers.
    */
   public:
      static void testSampledMovingAverage();
      static void testTwoTuple();
      static void testRegularAccumArray();

   /**
    * Other/uncategorized.
    */
   public:
      static void testNewtonSolver1D();
      static void testPdf();

};

#endif // TESTMATH_H
