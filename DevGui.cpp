#include "DevGui.h"

#include "AvailablePlotsList.h"
#include "Logger.h"

void DevGui::execute()
{
   Logger msg( "DevGui::execute" );
   msg << Msg::Info << "Running development code for the GUI..." << Msg::EndReq;

   Gui::AvailablePlotsList& plotsList = Gui::AvailablePlotsList::getInstance();

}
