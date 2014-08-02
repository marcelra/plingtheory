#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

class IThread;

class QListView;
class QPushButton;
class QStandardItemModel;

/// TODO: temporary includes for DummyPlotWidget
#include <QHBoxLayout>
#include <QLabel>

namespace Gui
{

/// TODO: doc
class DummyPlotWidget : public QWidget
{
   Q_OBJECT

   public:
      explicit DummyPlotWidget( QWidget* parent = 0 ) :
         QWidget( parent )
      {
         m_label = new QLabel( this );
         m_label->setText( "Not showing a plot" );

         QHBoxLayout* layout = new QHBoxLayout( this );
         layout->addWidget( m_label );

         setLayout( layout );
      }
      virtual ~DummyPlotWidget() {}

      QLabel*     m_label;
};

/// TODO: doc
class MainWindow : public QMainWindow
{
   Q_OBJECT

   public:
      explicit MainWindow( QWidget* parent = 0 );
      virtual ~MainWindow();

      void refreshPlotsList();

      void startWorkerThread( IThread* thread );

   private slots:
      void quitClickedSlot();
      void plotSelectedSlot( QModelIndex index );
      void lowFreqUpdate();
      void highFreqUpdate();

   private:
      QPushButton*         	m_quitButton;
      QListView*           	m_plotsListView;
      QStandardItemModel*     m_plotsModel;
      QWidget*             	m_plotWidget;
      QTimer*						m_lowFreqTimer;
      QTimer*						m_highFreqTimer;
      std::vector< IThread*>	m_runningThreads;
};

} /// namespace Gui

#endif // MAINWINDOW_H
