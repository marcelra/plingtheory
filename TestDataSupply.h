#ifndef TESTDATASUPPLY_H
#define TESTDATASUPPLY_H

#include "RealVector.h"

class TestDataSupply
{
   public:
      static RealVector createNoiseAndPeaks();
      static RealVector createNoiseAndPeaks( RealVector peakLocs, RealVector peakSigmas, RealVector peakAmpsAtMax );
};

#endif // TESTDATASUPPLY_H
