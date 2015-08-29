#ifndef DEVSUITE_H
#define DEVSUITE_H

class DevSuite
{
   public:
      static void execute();

   public:
      static void devIterateSrPeaks();
      static void devPeakSustainAlgorithm();
      static void devFundamentalFreqFinder();
      static void devTimeStretcher();
      static void devImprovedPeakAlgorithm();
      static void devSidelobeRejection();
};

#endif // DEVSUITE_H
