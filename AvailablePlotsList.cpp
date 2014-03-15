#include "AvailablePlotsList.h"

#include <QStandardItemModel>

namespace Gui
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AvailablePlotsList::AvailablePlotsList() :
   SingletonBase( "AvailablePlotsList" )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AvailablePlotsList::~AvailablePlotsList()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getInstance
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AvailablePlotsList& AvailablePlotsList::getInstance()
{
   if ( !s_instance )
   {
      s_instance = new AvailablePlotsList();
   }
   return *s_instance;
}

/// Singleton instance initialisation.
AvailablePlotsList* AvailablePlotsList::s_instance = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// addPlot
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AvailablePlotsList::addPlot( const std::string& name, Plotting::Plot2D* plot )
{
   m_plots.push_back( plot );
   m_plotNames.push_back( name );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// buildModel
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QStandardItemModel* AvailablePlotsList::buildModel() const
{
   QStandardItemModel* model = new QStandardItemModel();

   for ( size_t iPlot = 0; iPlot < m_plots.size(); ++iPlot )
   {
      QStandardItem* item = new QStandardItem();
      item->setText( m_plotNames[ iPlot ].c_str() );
      item->setData( QVariant::fromValue< Plotting::Plot2D* >( m_plots[ iPlot ] ) );
      item->setEditable( false );
      model->insertRow( iPlot, item );
   }
   return model;
}

} /// namespace Gui
