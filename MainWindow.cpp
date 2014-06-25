#include "MainWindow.h"

#include "AvailablePlotsList.h"
#include "IThread.h"
#include "Plot2D.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QStandardItemModel>
#include <QTimer>
#include <QVBoxLayout>

#include <cassert>

namespace Gui
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow( QWidget* parent ) :
   QMainWindow( parent )
{
   this->setGeometry( 0, 0, 1000, 700 );

   /// Build widgets.
   m_quitButton = new QPushButton( "Quit" );
   m_plotsListView = new QListView();

   /// Initialise widgets.
   QStandardItemModel* model = new QStandardItemModel( m_plotsListView );
   m_plotsListView->setModel( model );
   m_plotsListView->setFixedWidth( 300 );

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
   hLayout->addWidget( m_plotWidget, 1 );

   /// Make connections.
   connect( m_quitButton, SIGNAL( clicked() ), this, SLOT( quitClickedSlot() ) );
   connect( m_plotsListView, SIGNAL( activated( QModelIndex ) ), this, SLOT( plotSelectedSlot( QModelIndex ) ) );
   connect( m_plotsListView, SIGNAL( entered( QModelIndex ) ), this, SLOT( plotSelectedSlot( QModelIndex ) ) );
   connect( m_plotsListView, SIGNAL( pressed( QModelIndex ) ), this, SLOT( plotSelectedSlot( QModelIndex ) ) );

   m_lowFreqTimer  = new QTimer( this );
   m_lowFreqTimer->setInterval( 500 );
   connect( m_lowFreqTimer, SIGNAL( timeout() ), this, SLOT( lowFreqUpdate() ) );
   m_lowFreqTimer->start();

   m_highFreqTimer = new QTimer( this );
   m_highFreqTimer->setInterval( 10 );
   connect( m_highFreqTimer, SIGNAL( timeout() ), this, SLOT( highFreqUpdate() ) );
   m_highFreqTimer->start();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
   /// Clean up worker threads
   for ( size_t i = 0; i < m_runningThreads.size(); ++i )
   {
      m_runningThreads[ i ]->kill();
   }
   for ( size_t i = 0; i < m_runningThreads.size(); ++i )
   {
      delete m_runningThreads[ i ];
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// quitClickedSlot
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::quitClickedSlot()
{
   close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// plotSelectedSlot
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// buildPlotLists
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::buildPlotList()
{
   AvailablePlotsList& plotsProvider = AvailablePlotsList::getInstance();
   QStandardItemModel* model = plotsProvider.buildModel();

   if ( model )
   {
      QModelIndex currentIndex = m_plotsListView->currentIndex();
      delete m_plotsListView->model();
      m_plotsListView->setModel( model );
      m_plotsListView->setCurrentIndex( currentIndex );
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// startWorkerThread
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::startWorkerThread( IThread* thread )
{
   thread->start();
   m_runningThreads.push_back( thread );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// lowFreqUpdate
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::lowFreqUpdate()
{
   for ( auto it = m_runningThreads.begin(); it != m_runningThreads.end(); )
   {
      if ( !(*it)->isRunning() )
      {
         delete *it;
         it = m_runningThreads.erase( it );
      }
      else
      {
         ++it;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// updateThreadResults
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::highFreqUpdate()
{
   buildPlotList();
   AvailablePlotsList::getInstance().handleNewPlotRequest();
}

} /// namespace Gui
