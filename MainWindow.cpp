#include "MainWindow.h"

#include "AvailablePlotsList.h"
#include "Plot2D.h"

#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QStandardItemModel>
#include <QVBoxLayout>

#include <cassert>

namespace Gui
{

MainWindow::MainWindow( QWidget* parent ) :
   QMainWindow( parent )
{
   /// Build widgets.
   m_quitButton = new QPushButton( "Quit" );
   m_plotsListView = new QListView();

   /// Initialise widgets.
   QStandardItemModel* model = new QStandardItemModel( m_plotsListView );
   m_plotsListView->setModel( model );
   m_plotsListView->setFixedWidth( 200 );

   m_plotWidget = new DummyPlotWidget( this );
   m_plotWidget->setMinimumWidth( 400 );
   m_plotWidget->setMinimumHeight( 400 );

   /// Set layout.
   QWidget* centralWidget = new QWidget( this );
   setCentralWidget( centralWidget );

   QVBoxLayout* vLayout = new QVBoxLayout();
   vLayout->addWidget( m_plotsListView );
   vLayout->addWidget( m_quitButton );

   QHBoxLayout* hLayout = new QHBoxLayout();
   centralWidget->setLayout( hLayout );

   hLayout->addLayout( vLayout );
   hLayout->addWidget( m_plotWidget );

   /// Make connections.
   connect( m_quitButton, SIGNAL( clicked() ), this, SLOT( quitClickedSlot() ) );
   connect( m_plotsListView, SIGNAL( clicked( QModelIndex ) ), this, SLOT( plotSelectedSlot( QModelIndex ) ) );

   buildPlotList();
}

MainWindow::~MainWindow()
{}

void MainWindow::quitClickedSlot()
{
   close();
}

void MainWindow::plotSelectedSlot( QModelIndex index )
{
   QStandardItemModel* model = static_cast< QStandardItemModel* >( m_plotsListView->model() );
   QStandardItem* item = model->itemFromIndex( index );
   // m_plotWidget->m_label->setText( QString( "%1" ).arg( (long) item->data().value< Plotting::Plot2D* >() ) );

   QHBoxLayout* layout = dynamic_cast< QHBoxLayout* >( centralWidget()->layout() );
   assert( layout );
   // int plotLayoutIndex = layout->indexOf( m_plotWidget );
   // QLayoutItem* layoutItem = layout->takeAt( plotLayoutIndex );
   // layoutItem->
   layout->removeWidget( m_plotWidget );
   m_plotWidget->hide();
   m_plotWidget = item->data().value< Plotting::Plot2D* >();
   layout->addWidget( m_plotWidget, 1 );
   m_plotWidget->show();
   m_plotWidget->setMinimumWidth( 400 );
   m_plotWidget->setMinimumHeight( 400 );
}

void MainWindow::buildPlotList()
{
   delete m_plotsListView->model();
   AvailablePlotsList& plotsProvider = AvailablePlotsList::getInstance();

   QStandardItemModel* model = plotsProvider.buildModel();

   m_plotsListView->setModel( model );
}

} /// namespace Gui
