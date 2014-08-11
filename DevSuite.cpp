#include "DevGui.h"
#include "DevSuite.h"
#include "Logger.h"
#include "TestDataSupply.h"
#include "Utils.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// execute
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevSuite::execute()
{
   Logger msg( "DevSuite" );
   msg << Msg::Info << "Running development code..." << Msg::EndReq;

   devAnalysisSrpa();

   return;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Include section
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "AnalysisSrpa.h"

void DevSuite::devAnalysisSrpa()
{
   Logger msg( "devAnalysisSrpa" );
   msg << Msg::Info << "In devAnalysisSrpa..." << Msg::EndReq;

   Analysis::AnalysisSrpa anaAlg;

   // const RealVector& frequencies = Utils::createRangeReal( 50, 10000, 500 );
   const RealVector& frequencies = realVector( 100, 400, 1000, 10000 );
   const RealVector& deltaFreq = Utils::createRangeReal( -500, 500, 4001 );
   // anaAlg.studyFrequencyPerformance( frequencies );

   anaAlg.studyFrequencyProximity( frequencies, deltaFreq, 1 );

}
