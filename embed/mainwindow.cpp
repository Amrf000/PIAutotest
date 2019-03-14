#include "mainwindow.h"
#include "widgetcontainer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(WidgetContainer::createContainer(this,"vncviewer.exe"));
}

MainWindow::~MainWindow()
{

}
