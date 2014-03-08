#ifndef AVAILABLEPLOTSLIST_H
#define AVAILABLEPLOTSLIST_H

#include "SingletonBase.h"

namespace Gui
{

class AvailablePlotsList : public SingletonBase
{
   public:
      static AvailablePlotsList& getInstance();

   private:
      AvailablePlotsList();
      virtual ~AvailablePlotsList();

      static AvailablePlotsList*     s_instance;
};

} /// namespace Gui

#endif // AVAILABLEPLOTSLIST_H
