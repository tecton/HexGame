#include <QtGui/QApplication>
#include <QTextCodec>
#include <time.h>
#include "mainwidget.h"

#include "gamerecord.h"
#include "statistic.h"
#include "achievements.h"
#include "publicgamesounds.h"

GameRecord gameRecord;
Statistic statistic;
Achievements achievements;

int main(int argc, char *argv[])
{
  PublicGameSounds::init();

  srand ( time(NULL) );

  QTextCodec *codec = QTextCodec::codecForName("utf8");
  QTextCodec::setCodecForLocale(codec);
  QTextCodec::setCodecForCStrings(codec);
  QTextCodec::setCodecForTr(codec);
  QApplication a(argc, argv);

  QObject::connect(&statistic,
                   SIGNAL(statisticChanged(Statistic::StatisticType,int,int)),
                   &achievements,
                   SLOT(statisticChanged(Statistic::StatisticType,int,int)));

  MainWidget w;

//  w.setAttribute(Qt::WA_PaintOnScreen);
  w.setGeometry(QRect(100,100,1024,600));

  // Accept touch event so that we can use 2 mouses
  w.setAttribute(Qt::WA_AcceptTouchEvents);

  w.show();

  return a.exec();
}
