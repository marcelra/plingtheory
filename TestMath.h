#ifndef TESTMATH_H
#define TESTMATH_H

/// TODO: doc
class TestMath
{
   public:
      static void execute();

   private:
      static void testTwoDimExampleObjective();
      static void testGradDescOptimiser();

   private:
      static void testSampledMovingAverage();
      static void testTwoTuple();
      static void testRegularAccumArray();

   private:
      static void testNewtonSolver1D();

};

#endif // TESTMATH_H
