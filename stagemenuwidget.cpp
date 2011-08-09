#include "stagemenuwidget.h"

#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QFileInfo>
#include <QTextStream>
#include "basicpainter.h"
#include "gamemath.h"
#include "stagemenuitems.h"
#include "rotatepuzzlegame.h"
#include "puzzlegameinit.h"

#include <QMessageBox>
#include <QDebug>

#define MAIN_MENU_LOGICAL_WIDTH  800
#define MAIN_MENU_LOGICAL_HEIGHT 500

int errrrr[] = {
          0,  0,  0,  0,  0,
        0,  3,  7,  7,  0,  0,
      0,  7,  3,  7,  7,  7,  0,
    0,  7,  7,  2,  7,  7,  7,  0,
  0,  0,  7,  7,  1,  2,  3,  3,  0,
    0,  7,  7,  2,  7,  7,  7,  0,
      0,  7,  3,  7,  7,  7,  0,
        0,  3,  7,  7,  0,  0,
          0,  0,  0,  0,  0
};

// useless function which is combined into puzzlegameinit.cpp

//void getStageBallIndex(int *ballIndex,int *toBeIndex, int index, int type)
//{
//  QString filename;
//  if (type == 0)
//    filename = ":/stages/exchange";
//  filename = QObject::tr("%1%2").arg(filename).arg(index + 1);
//  QFile file(filename);
//  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//    return;
//  QTextStream in(&file);
//  for (int i = 0; i < 61; ++i)
//  {
//    in >> ballIndex[i];
//  }
//  for (int i = 0; i < 61; ++i)
//  {
//    in >> toBeIndex[i];
//  }
//}

void ExchangeStageMenuWidget::makePixmap(QPixmap& pixmap, int width, int height)
{
  makeBasicPixmap(pixmap, width, height);
//  QMessageBox::critical(0,"","Basic made");
  addEffect(pixmap, width, height);
}

void ExchangeStageMenuWidget::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
  pixmap = QPixmap(width, height);
  pixmap.fill(Qt::black);
  QPainter *painter = new QPainter(&pixmap);
  //QMessageBox::critical(0,"","Try to paint items");
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           frameCount);

//  QMessageBox::critical(0,"","Items painted");
  painter->end();
  delete painter;
}

void ExchangeStageMenuWidget::addEffect(QPixmap& pixmap, int width, int height)
{

}

QPointF ExchangeStageMenuWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * MAIN_MENU_LOGICAL_WIDTH,
                 yRate * MAIN_MENU_LOGICAL_HEIGHT);
}

ExchangeStageMenuWidget::ExchangeStageMenuWidget() :
    frameCount(0)
{
  stageItem = new StageMenuItem *[10];
  for (int i = 0; i < 10; ++i)
  {
    stageItem[i] = new StageMenuItem(":/images/mainmenuitems/swapclassicgame*.png",
                                     1);
    stageItem[i]->setPos(QPointF(0.1 + 0.2 * (i % 5), 0.3 + 0.5 * (i / 5)));
    myItems.push_back(stageItem[i]);
  }

  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
  t->start();
}
void ExchangeStageMenuWidget::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  if (button == Qt::RightButton)
  {
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }
  for (int i = 0; i < 10; ++i)
  {
    if (distanceOfTwoPoints(mousePos,
                            QPointF((0.1 + 0.2 * (i % 5)) * MAIN_MENU_LOGICAL_WIDTH,
                                    (0.3 + 0.5 * (i / 5)) * MAIN_MENU_LOGICAL_HEIGHT)) < 50)
    {
//      int *ballIndex = new int [61];
//      int *toBeIndex = new int [61];
//      getStageBallIndex(ballIndex, toBeIndex, i, 0);
//      RotatePuzzleGame *puzzleGame = new RotatePuzzleGame(ballIndex, toBeIndex);
      RotatePuzzleGame *puzzleGame = PuzzleGameInit::initRotatePuzzleGame(i, 0);
      emit giveControlTo(puzzleGame, false);
    }
  }
}

void ExchangeStageMenuWidget::dealMoved(QPointF mousePos, Qt::MouseButton button)
{

}

void ExchangeStageMenuWidget::dealReleased(QPointF mousePos, Qt::MouseButton button)
{

}

void ExchangeStageMenuWidget::advance()
{
  ++frameCount;
  frameCount = frameCount % 32;
  //effectPainter->advance();
}

ExchangeStageMenuWidget::~ExchangeStageMenuWidget()
{
  t->stop();
  delete t;
  delete stageItem;
  for (QVector<AbstractItem *>::Iterator itr = myItems.begin();
       itr != myItems.end(); ++itr)
    delete *itr;
}

/////////////////////////////////////////////////////////////////////////////////

UniteStageMenuWidget::UniteStageMenuWidget() :
    frameCount(0)
{
  stageItem = new StageMenuItem *[10];
  for (int i = 0; i < 10; ++i)
  {
    stageItem[i] = new StageMenuItem(":/images/mainmenuitems/swapclassicgame*.png",
                                     1);
    stageItem[i]->setPos(QPointF(0.1 + 0.2 * (i % 5), 0.3 + 0.5 * (i / 5)));
    myItems.push_back(stageItem[i]);
  }

  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
  t->start();
}
void UniteStageMenuWidget::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  if (button == Qt::RightButton)
  {
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }
  for (int i = 0; i < 10; ++i)
  {
    if (distanceOfTwoPoints(mousePos,
                            QPointF((0.1 + 0.2 * (i % 5)) * MAIN_MENU_LOGICAL_WIDTH,
                                    (0.3 + 0.5 * (i / 5)) * MAIN_MENU_LOGICAL_HEIGHT)) < 50)
    {
//      int *ballIndex = new int [61];
//      int *toBeIndex = new int [61];
//      getStageBallIndex(ballIndex, toBeIndex, i, 0);
//      RotatePuzzleGame *puzzleGame = new RotatePuzzleGame(ballIndex, toBeIndex);
      RotatePuzzleGame *puzzleGame = PuzzleGameInit::initRotatePuzzleGame(i, 1);
      emit giveControlTo(puzzleGame, false);
    }
  }
}

void UniteStageMenuWidget::dealMoved(QPointF mousePos, Qt::MouseButton button)
{

}

void UniteStageMenuWidget::dealReleased(QPointF mousePos, Qt::MouseButton button)
{

}

void UniteStageMenuWidget::advance()
{
  ++frameCount;
  frameCount = frameCount % 32;
  //effectPainter->advance();
}

UniteStageMenuWidget::~UniteStageMenuWidget()
{
  t->stop();
  delete t;
  delete stageItem;
  for (QVector<AbstractItem *>::Iterator itr = myItems.begin();
       itr != myItems.end(); ++itr)
    delete *itr;
}

void UniteStageMenuWidget::makePixmap(QPixmap& pixmap, int width, int height)
{
  makeBasicPixmap(pixmap, width, height);
//  QMessageBox::critical(0,"","Basic made");
  addEffect(pixmap, width, height);
}

void UniteStageMenuWidget::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
  pixmap = QPixmap(width, height);
  pixmap.fill(Qt::black);
  QPainter *painter = new QPainter(&pixmap);
  //QMessageBox::critical(0,"","Try to paint items");
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           frameCount);

//  QMessageBox::critical(0,"","Items painted");
  painter->end();
  delete painter;
}

void UniteStageMenuWidget::addEffect(QPixmap& pixmap, int width, int height)
{

}

QPointF UniteStageMenuWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * MAIN_MENU_LOGICAL_WIDTH,
                 yRate * MAIN_MENU_LOGICAL_HEIGHT);
}

/////////////////////////////////////////////////////////////////////////////////

LockStageMenuWidget::LockStageMenuWidget() :
    frameCount(0)
{
  stageItem = new StageMenuItem *[10];

  //test 10 stage in one png
  stageItem[0] = new StageMenuItem(":/images/mainmenuitems/lock_stage*.png", 1);
  stageItem[0]->setPos(QPointF(0.05, 0.05));
  myItems.push_back(stageItem[0]);

//  for (int i = 0; i < 10; ++i)
//  {
//    stageItem[i] = new StageMenuItem(":/images/mainmenuitems/lock_stage*.png",
//                                     1);
//    stageItem[i]->setPos(QPointF(0.1 + 0.2 * (i % 5), 0.3 + 0.5 * (i / 5)));
//    myItems.push_back(stageItem[i]);
//  }

  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
  t->start();
}
void LockStageMenuWidget::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  if (button == Qt::RightButton)
  {
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }
  for (int i = 0; i < 10; ++i)
  {
    if (distanceOfTwoPoints(mousePos,
                            QPointF((0.1 + 0.2 * (i % 5)) * MAIN_MENU_LOGICAL_WIDTH,
                                    (0.3 + 0.5 * (i / 5)) * MAIN_MENU_LOGICAL_HEIGHT)) < 50)
    {
//      int *ballIndex = new int [61];
//      int *toBeIndex = new int [61];
//      getStageBallIndex(ballIndex, toBeIndex, i, 0);
//      RotatePuzzleGame *puzzleGame = new RotatePuzzleGame(ballIndex, toBeIndex);
      RotatePuzzleGame *puzzleGame = PuzzleGameInit::initRotatePuzzleGame(i, 2);
      emit giveControlTo(puzzleGame, false);
    }
  }
}

void LockStageMenuWidget::dealMoved(QPointF mousePos, Qt::MouseButton button)
{

}

void LockStageMenuWidget::dealReleased(QPointF mousePos, Qt::MouseButton button)
{

}

void LockStageMenuWidget::advance()
{
  ++frameCount;
  frameCount = frameCount % 32;
  //effectPainter->advance();
}

LockStageMenuWidget::~LockStageMenuWidget()
{
  t->stop();
  delete t;
  delete stageItem;
  for (QVector<AbstractItem *>::Iterator itr = myItems.begin();
       itr != myItems.end(); ++itr)
    delete *itr;
}

void LockStageMenuWidget::makePixmap(QPixmap& pixmap, int width, int height)
{
  makeBasicPixmap(pixmap, width, height);
//  QMessageBox::critical(0,"","Basic made");
  addEffect(pixmap, width, height);
}

void LockStageMenuWidget::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
  pixmap = QPixmap(width, height);
  pixmap.fill(Qt::black);
  QPainter *painter = new QPainter(&pixmap);
  //QMessageBox::critical(0,"","Try to paint items");
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           frameCount);

//  QMessageBox::critical(0,"","Items painted");
  painter->end();
  delete painter;
}

void LockStageMenuWidget::addEffect(QPixmap& pixmap, int width, int height)
{

}

QPointF LockStageMenuWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * MAIN_MENU_LOGICAL_WIDTH,
                 yRate * MAIN_MENU_LOGICAL_HEIGHT);
}
