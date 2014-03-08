#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

class QListView;
class QPushButton;

/// TODO: temporary includes for DummyPlotWidget
#include <QHBoxLayout>
#include <QLabel>

namespace Gui
{

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

class MainWindow : public QMainWindow
{
   Q_OBJECT

   public:
      explicit MainWindow( QWidget* parent = 0 );
      virtual ~MainWindow();

      void buildPlotList();

   private slots:
      void quitClickedSlot();
      void plotSelectedSlot( QModelIndex index );

   private:
      QPushButton*         m_quitButton;
      QListView*           m_plotsListView;
      DummyPlotWidget*     m_plotWidget;
};

} /// namespace Gui

#endif // MAINWINDOW_H
