#ifndef DEVSUITE_H
#define DEVSUITE_H

class DevSuite
{
   public:
      static void execute();

   public:
      static void devPeakFinder2();
      static void devRebinSRSpec();

      static void devFourierTemplates();
      static void devSamples();

   /// Parked.
   public:
      static void devSidelobeSubtraction();
      static void devEntropyPeaks();

   /// Parked and interesting.
   public:
      static void devFourierPeakFinder1();

};

#endif // DEVSUITE_H
