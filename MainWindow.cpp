#include "MainWindow.h"

#include "AvailablePlotsList.h"
#include "IThread.h"
#include "Plot2D.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardItemModel>
#include <QTimer>
#include <QVBoxLayout>

#include <cassert>
#include <sstream>

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
   m_plotsListView->setSelectionMode( QListView::SingleSelection );
   m_plotsListView->setAlternatingRowColors( true );
   m_plotsListView->setModel( new QStandardItemModel( this ) );

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
   connect( m_plotsListView->selectionModel(), SIGNAL( currentChanged( QModelIndex, QModelIndex ) ), this, SLOT( plotSelectedSlot( QModelIndex, QModelIndex ) ) );


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
void MainWindow::plotSelectedSlot( const QModelIndex& index, const QModelIndex& /*previousIndex*/ )
{
   QStandardItemModel* model = static_cast< QStandardItemModel* >( m_plotsListView->model() );
   QStandardItem* item = model->itemFromIndex( index );

   QHBoxLayout* layout = dynamic_cast< QHBoxLayout* >( centralWidget()->layout() );
   assert( layout );

   layout->removeWidget( m_plotWidget );
   m_plotWidget->hide();
   m_plotWidget = item->data().value< Plotting::Plot2D* >();
   layout->addWidget( m_plotWidget, 1 );
   m_plotWidget->show();
   m_plotWidget->setMinimumWidth( 400 );
   m_plotWidget->setMinimumHeight( 400 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// refreshPlotsList
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::refreshPlotsList()
{
   AvailablePlotsList& plotsProvider = AvailablePlotsList::getInstance();
   plotsProvider.updateModel( dynamic_cast< QStandardItemModel* >( m_plotsListView->model() ) );
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
         if ( (*it)->getReturnStatus() == IThread::Finished )
         {
            delete *it;
            it = m_runningThreads.erase( it );
         }
         else
         {
            std::ostringstream msg;
            msg << "Operation " << *it << " failed.";
            QMessageBox::warning( this, "Error", msg.str().c_str() );

            delete *it;
            it = m_runningThreads.erase( it );
         }
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
   refreshPlotsList();
   AvailablePlotsList::getInstance().handleNewPlotRequest();
}

} /// namespace Gui
