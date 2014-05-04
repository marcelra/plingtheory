#ifndef TESTSUITE_H
#define TESTSUITE_H

#include "RawPcmData.h"

/**
 * @class TestSuite
 * @brief Collection of test methods
 */
class TestSuite
{
   public:
      /**
       * TestSuite main entry points
       */
      static void execute();
      static void runTestMath();
      static void singleTest();

      /**
       * Infrastructure
       */
      static void testObjectPool();
      static void testThreading();

      /**
       * Utilities
       */
      static void testFindMinima();
      static void testRandomNumberGenerator();

      /**
       * Data framework
       */
      static void testSoundData();
      static void testWaveFile();
      static void testNote();

      /**
       * Development algorithms (try-out area)
       */
      static void testEffectTrianglizer();
      static void testPartialIntegralTransform();

      /**
       * Music generation tests
       */
      static void testSineGenerator();
      static void testNoiseGenerator();
      static void testTriangleGenerator();
      static void testSawtoothGenerator();
      static void testEnvelope();
      static void testRandomMusic();

      /**
       * Fourier algorithms
       */
      static void testDynamicFourier();
      static void testAdvancedFourier();
      static void testStftAlgorithm();
      static void testSpectralReassignment();

      /**
       * FFTW algorithms
       */
      static void testFftw();

      /**
       * Feature algorithms
       */
      static void testPeakDetection();

      /**
       * Integration (coupling of algorithms)
       */
      static void testIntegration();

      /**
       * Plotting
       */
      static void testHist2D();
      static void testHistogram();

   /**
    * Helper methods
    */
   private:
      /**
       * Generate some wave data
       */
      static RawPcmData::Ptr generateRandomMusic();
};

#endif // TESTSUITE_H
