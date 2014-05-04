#ifndef DEVSUITE_H
#define DEVSUITE_H

class DevSuite
{
   public:
      static void execute();

   /**
    * Plotting
    */
   public:
      static void devHistogram();

   public:
      static void devPeakFinder2();
      static void devRebinSRSpec();

      static void devSamples();

      static void devMlp();
      static void devMlp2();

      static void devHist2D();

   /**
    * Math
    */
   public:
      static void testPdf();
      static void devNewtonSolver1D();

   /// Parked.
   public:
      static void devSidelobeSubtraction();
      static void devEntropyPeaks();

   /// Parked and interesting.
   public:
      static void devFourierPeakFinder1();
};

#endif // DEVSUITE_H
