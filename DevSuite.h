#ifndef DEVSUITE_H
#define DEVSUITE_H

class DevSuite
{
   public:
      static void execute();

   public:
      static void devSidelobeSubtraction();
      static void devPeakFinder();
      static void devFourierPeakFinder1();
      static void devFourierPeakFinder2();

      static void devTwoTuple();
      static void devRebinSRSpec();
};

#endif // DEVSUITE_H
