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
      static void runCurrentDevelopmentTest();
      static void runAllTests();
      static void runTestMath();

      /**
       * Infrastructure
       */
      static void testObjectPool();
      static void testAlgorithmFramework();

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
      static void testEnvelope();
      static void testRandomMusic();

      /**
       * Fourier algorithms
       */
      static void testFourierFilter();
      static void testFourierNoteFilter();
      static void testFourierNoteFilterExternalFile();
      static void testWaveletTransform();
      static void testDynamicFourier();
      static void testAdvancedFourier();
      static void testStftAlgorithm();

      /**
       * FFTW algorithms
       */
      static void testFftw();
      static void testShortTimeFftw();

      /**
       * Feature data
       */
      static void testFeatureData();

      /**
       * Feature algorithms
       */
      static void testPeakDetection();

      /**
       * Integration (coupling of algorithms)
       */
      static void testIntegration();

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
