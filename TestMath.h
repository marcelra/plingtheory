#ifndef TESTMATH_H
#define TESTMATH_H

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

};

#endif // TESTMATH_H
