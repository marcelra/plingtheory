#include "AvailablePlotsList.h"

namespace Gui
{

AvailablePlotsList::AvailablePlotsList() :
   SingletonBase( "AvailablePlotsList" )
{}

AvailablePlotsList::~AvailablePlotsList()
{}

AvailablePlotsList& AvailablePlotsList::getInstance()
{
   if ( !s_instance )
   {
      s_instance = new AvailablePlotsList();
   }
   return *s_instance;
}

AvailablePlotsList* AvailablePlotsList::s_instance = 0;


} /// namespace Gui
