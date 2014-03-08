#include "DevGui.h"

#include "AvailablePlotsList.h"
#include "Logger.h"
#include "StftAlgorithm.h"
#include "TestDataSupply.h"

#include "RegLargeDataCurve.h"
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

   Plotting::RegLargeDataCurve* curveItem = new Plotting::RegLargeDataCurve( xData, *yData );

   Plotting::Plot2D* plot2D = new Plotting::Plot2D();
   plot2D->addItem( curveItem );

   plotsList.addPlot( "CurrentTestSample", plot2D );

   WaveAnalysis::StftAlgorithm stftAlg( data->getSamplingInfo() );
   WaveAnalysis::StftData::Ptr stft = stftAlg.execute( *data );
   const WaveAnalysis::FourierSpectrum& spec = stft->getSpectrum( 0 );
   Plotting::CurveItem* specItem = new Plotting::CurveItem( spec.getFrequencies(), spec.getMagnitude() );
   specItem->setAntialiasing( true );
   plot2D = new Plotting::Plot2D();

   plot2D->addItem( specItem );
   plotsList.addPlot( "Fourier transform", plot2D );

}
