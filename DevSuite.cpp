#include "DevSuite.h"
#include "Logger.h"
#include "TestDataSupply.h"

#include "TriangleGenerator.h"
#include "KernelPdf.h"
#include "SpectralReassignmentTransform.h"
#include "MonophonicSimpleRandomMusicGenerator.h"
#include "IPlotFactory.h"
#include "PredefinedRealFunctions.h"
#include "GaussPdf.h"
#include "SortCache.h"
#include "SineGenerator.h"
#include "SampledMovingAverage.h"
#include "SawtoothGenerator.h"
#include "LinearInterpolator.h"
#include "Utils.h"
#include "SrSpecPeakAlgorithm.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// execute
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::execute()
{
   Logger msg( "DevSuite" );
   msg << Msg::Info << "Running development code..." << Msg::EndReq;

   return;
}
