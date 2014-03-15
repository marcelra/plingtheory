#ifndef AVAILABLEPLOTSLIST_H
#define AVAILABLEPLOTSLIST_H

#include "SingletonBase.h"
#include "Plot2D.h"

#include <vector>

class QStandardItemModel;

namespace Gui
{

/**
 * @class AvailablePlotsList
 * @brief Stores the plots that have been created with the PlotFactory and serves as a model builder for the GUI.
 * This class is also responsible for filling the model underlying the list view that displays all the available plots.
 */
class AvailablePlotsList : public SingletonBase
{
   public:
      /**
       * Access singleton instance.
       */
      static AvailablePlotsList& getInstance();

      /**
       * Add a plot.
       */
      void addPlot( const std::string& name, Plotting::Plot2D* plot );
      /**
       * Build the Qt model for the QListView.
       */
      QStandardItemModel* buildModel() const;

   private:
      /**
       * Private constructor.
       */
      AvailablePlotsList();
      /**
       * Destructor.
       */
      virtual ~AvailablePlotsList();

      static AvailablePlotsList*     s_instance;      //! Singleton instance.

  private:
      std::vector< Plotting::Plot2D* > m_plots;       //! Store of plots.
      std::vector< std::string >       m_plotNames;   //! Names of plots.
};

} /// namespace Gui

#endif // AVAILABLEPLOTSLIST_H
