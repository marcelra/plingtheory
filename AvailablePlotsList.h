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
      Plotting::Plot2D* addPlot( const std::string& name );
      /**
       * Will fill the model, @param[out] model, with plots. Returns immediately if a plot is being requested.
       */
      void updateModel( QStandardItemModel* model );

      /**
       * Creates new plots. This method is intended to be called regularly from the main (GUI) thread.
       * The Qt object should be created on the main thread otherwise they cannot handle signals etc.
       * This method is protected by a mutex and only one new plot can be requested. This method should not be
       * called from the main thread.
       */
      void handleNewPlotRequest();

   private:
      /**
       * Private constructor.
       */
      AvailablePlotsList();
      /**
       * Destructor.
       */
      virtual ~AvailablePlotsList();

      /**
       * Request the main thread to create a new default plot.
       */
      Plotting::Plot2D* requestNewPlot();

   private:
      static AvailablePlotsList*     s_instance;            //! Singleton instance.

   private:
      std::vector< Plotting::Plot2D* > m_plots;             //! Store of plots.
      std::vector< std::string >       m_plotNames;         //! Names of plots.
      bool                             m_newPlotRequested;  //! True iff a new plot is requested.
      bool                             m_newPlotReady;      //! True iff a new plot is created.
      Plotting::Plot2D*                m_newPlot;           //! Pointer to the new plot.
};

} /// namespace Gui

#endif // AVAILABLEPLOTSLIST_H
