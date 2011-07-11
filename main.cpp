#include <QtGui/QApplication>
#include <time.h>
#include "mainwindow.h"
#include "gameviewwidget.h"

int main(int argc, char *argv[])
{
  srand ( time(NULL) );
    QApplication a(argc, argv);
//    MainWindow w;
    GameViewWidget w;
    w.setGeometry(50,50,850,600);
    w.show();

    return a.exec();
}
