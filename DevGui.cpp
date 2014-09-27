#include "DevGui.h"

#include "AvailablePlotsList.h"
#include "IPlotFactory.h"
#include "Logger.h"
#include "RegLargeDataCurve.h"
#include "StftAlgorithm.h"
#include "TestDataSupply.h"
#include "Utils.h"

#include <boost/thread.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// execute
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevGui::execute()
{
   // devPlotFactory();
   // devMakeALotOfPlots();
   devPlotExport();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devPlotFactory
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevGui::devPlotFactory()
{
   RawPcmData::Ptr data = TestDataSupply::getCurrentTestSample();
   WaveAnalysis::StftAlgorithm stftAlg( data->getSamplingInfo() );
   WaveAnalysis::StftData::Ptr stft = stftAlg.execute( *data );

   gPlotFactory().createPlot( "STFT graph" );
   gPlotFactory().createStftGraph( *stft );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// makeALotOfPlots
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevGui::devMakeALotOfPlots()
{
   for ( size_t i = 0; i < 200; ++i )
   {
      std::ostringstream plotTitle;
      plotTitle << "plot" << i;
      gPlotFactory().createPlot( plotTitle.str() );
      gPlotFactory().createGraph( realVector( 0, 1 ), realVector( i, i ) );

      boost::this_thread::sleep_for( boost::chrono::milliseconds( 50 ) );
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// devPlotExport
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DevGui::devPlotExport()
{
   RealVector x = Utils::createRangeReal( -2, 2, 100 );
   RealVector y( x.size() );
   for ( size_t i = 0; i < x.size(); ++i )
   {
      y[ i ] = x[ i ] * x[ i ];
   }

   Plotting::Plot2D& plot = gPlotFactory().createPlot( "NewPlot" );
   gPlotFactory().createGraph( x, y );

   plot.setXAxisTitle( "This is the x-axis" );
   plot.setYAxisTitle( "This is the y-axis" );
}
