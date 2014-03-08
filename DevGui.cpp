#include "DevGui.h"

#include "AvailablePlotsList.h"
#include "Logger.h"
#include "TestDataSupply.h"

#include "CurveItem.h"
#include "Plot2D.h"

void DevGui::execute()
{
   Logger msg( "DevGui::execute" );
   msg << Msg::Info << "Running development code for the GUI..." << Msg::EndReq;

   Gui::AvailablePlotsList& plotsList = Gui::AvailablePlotsList::getInstance();

   RawPcmData::Ptr data = TestDataSupply::getCurrentTestSample();
   const RealVectorPtr yData = data->copyToVectorData();

   RealVector xData( yData->size() );
   for ( size_t i = 0; i < yData->size(); ++i )
   {
      xData[ i ] = i;
   }

   Plotting::CurveItem* curveItem = new Plotting::CurveItem( xData, *yData );

   Plotting::Plot2D* plot2D = new Plotting::Plot2D();
   plot2D->addItem( curveItem );

   plotsList.addPlot( "CurrentTestSample", plot2D );

}
