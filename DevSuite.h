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

      static void devRootSaveAll();
      static void devTwoTuple();
};

#endif // DEVSUITE_H
