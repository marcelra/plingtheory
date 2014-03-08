#include "MainWindow.h"

#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QStandardItemModel>
#include <QVBoxLayout>

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

   m_plotWidget = new DummyPlotWidget( this );
   m_plotWidget->setFixedWidth( 400 );

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
   m_plotWidget->m_label->setText( QString( "%1" ).arg( item->data().toInt() ) );
}

void MainWindow::buildPlotList()
{
   delete m_plotsListView->model();
   QStandardItemModel* model = new QStandardItemModel( m_plotsListView );

   QStandardItem* item = new QStandardItem();
   item->setText( "Test" );
   item->setData( QVariant( 1 ) );
   item->setEditable( false );
   model->insertRow( 0, item );

   m_plotsListView->setModel( model );
}

} /// namespace Gui
