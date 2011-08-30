#include <QtGui/QApplication>
#include <QTextCodec>
#include <time.h>
//#include <QDebug>
//#include "mainwindow.h"
#include "sixtyonegameboardinfo.h"
#include "rules.h"
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

//  AbstractRule *rule = new SwapClassicGameRule();
//  AbstractGameBoardInfo *gameboardInfo = new SixtyOneGameBoardInfo();
//  MainWindow w(rule, gameboardInfo);
//  w.setWindowTitle("Swap");
//  w.show();

//  AbstractRule *rule2 = new RotateClassicGameRule();
//  AbstractGameBoardInfo *gameboardInfo2 = new SixtyOneGameBoardInfo();
//  MainWindow w2(rule2, gameboardInfo2);
//  w2.setWindowTitle("Rotate");
//  w2.show();
//  for (int i = 0;i < gameboardInfo->totalBallCounts();++i)
//    qDebug() << i << gameboardInfo->positionOfIndex(i);
  MainWidget w;
  w.setGeometry(QRect(100,100,1024,600));
  w.show();

  return a.exec();
}
