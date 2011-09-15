#include <QtGui/QApplication>
#include <QTextCodec>
#include <time.h>
#include "mainwidget.h"

#include "gamerecord.h"

GameRecord gameRecord;

int main(int argc, char *argv[])
{
  srand ( time(NULL) );

  QTextCodec *codec = QTextCodec::codecForName("utf8");
  QTextCodec::setCodecForLocale(codec);
  QTextCodec::setCodecForCStrings(codec);
  QTextCodec::setCodecForTr(codec);
  QApplication a(argc, argv);

  MainWidget w;
  w.setGeometry(QRect(100,100,1024,600));
  w.show();

  return a.exec();
}
