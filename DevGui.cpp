#include "DevGui.h"

#include "AvailablePlotsList.h"
#include "Logger.h"
#include "StftAlgorithm.h"
#include "TestDataSupply.h"

#include "RegLargeDataCurve.h"
#include "CurveItem.h"
#include "ScatterItem.h"
#include "Plot2D.h"

#include "IPlotFactory.h"

void DevGui::execute()
{
   devPlotFactory();
}

void DevGui::devPlotFactory()
{
   RawPcmData::Ptr data = TestDataSupply::getCurrentTestSample();
   WaveAnalysis::StftAlgorithm stftAlg( data->getSamplingInfo() );
   WaveAnalysis::StftData::Ptr stft = stftAlg.execute( *data );

   gPlotFactory().createPlot( "STFT graph" );
   gPlotFactory().createStftGraph( *stft );
}
