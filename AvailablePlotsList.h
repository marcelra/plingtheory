#ifndef AVAILABLEPLOTSLIST_H
#define AVAILABLEPLOTSLIST_H

#include "SingletonBase.h"
#include "Plot2D.h"

#include <vector>

class QStandardItemModel;

namespace Gui
{

class AvailablePlotsList : public SingletonBase
{
   public:
      static AvailablePlotsList& getInstance();

      void addPlot( const std::string& name, Plotting::Plot2D* plot );
      QStandardItemModel* buildModel() const;

   private:
      AvailablePlotsList();
      virtual ~AvailablePlotsList();

      static AvailablePlotsList*     s_instance;

  private:
      std::vector< Plotting::Plot2D* > m_plots;
      std::vector< std::string >       m_plotNames;
};

} /// namespace Gui

#endif // AVAILABLEPLOTSLIST_H
