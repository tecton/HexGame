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

#define LOGICAL_WIDTH  800
#define LOGICAL_HEIGHT 500

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
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           frameCount);

  painter->end();
  delete painter;
}

void ExchangeStageMenuWidget::addEffect(QPixmap& pixmap, int width, int height)
{

}

QPointF ExchangeStageMenuWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}

ExchangeStageMenuWidget::ExchangeStageMenuWidget(int stageType) :
  frameCount(0)
{
  type = stageType;
  stageItem = new StageMenuItem *[5];
  prefix = ":/images/stageitems/stage_img";
  suffix = "*.png";
  imageName = "";
  QString name[] = {"01", "02", "03", "04"};
  position = new QPointF[5];
  position[0] = QPointF(0.35, 0.35);
  position[1] = QPointF(0.5, 0.2);
  position[2] = QPointF(0.5, 0.5);
  position[3] = QPointF(0.65, 0.35);
  position[4] = QPointF(0.5, 0.8);

  for (int i = 0; i < 4; ++i)
  {
    imageName = QObject::tr("%1%2%3").arg(prefix).arg(name[i]).arg(suffix);
    stageItem[i] = new StageMenuItem(imageName);
    stageItem[i]->setPos(position[i]);
    myItems.push_back(stageItem[i]);
  }

  if (type == 0)
    stageItem[4] = new StageMenuItem(":/images/stageitems/button_advance*.png");
  else
    stageItem[4] = new StageMenuItem(":/images/stageitems/button_normal*.png");
  stageItem[4]->setPos(position[4]);
  myItems.push_back(stageItem[4]);

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
  for (int i = 0; i < 4; ++i)
  {
    if (distanceOfTwoPoints(mousePos,
                            toScene(position[i].x(), position[i].y())) < 81)
    { 
//      RotatePuzzleGame *puzzleGame = PuzzleGameInit::initRotatePuzzleGame(i, 0);
//      emit giveControlTo(puzzleGame, false);
      if (type == 0)
        emit giveControlTo(PuzzleGameInit::initRotatePuzzleGame(i, 0), false);
      else
        emit giveControlTo(PuzzleGameInit::initRotatePuzzleGame(i, 1), false);
    }
  }
  if (distanceOfTwoPoints(mousePos,
                          toScene(position[4].x(), position[4].y())) < 81)
  {
    emit giveControlTo(new ExchangeStageMenuWidget(type ^ 1), true);
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

UniteStageMenuWidget::UniteStageMenuWidget(int stageType) :
    frameCount(0)
{
  type = stageType;
  stageItem = new StageMenuItem *[6];
  prefix = ":/images/stageitems/stage_img";
  suffix = "*.png";
  imageName = "";
  QString name[] = {"01", "02", "03", "04", "05"};
  position = new QPointF[6];
  position[0] = QPointF(0.35, 0.65);
  position[1] = QPointF(0.35, 0.35);
  position[2] = QPointF(0.5, 0.2);
  position[3] = QPointF(0.65, 0.35);
  position[4] = QPointF(0.65, 0.65);
  position[5] = QPointF(0.5, 0.8);

  for (int i = 0; i < 5; ++i)
  {
    imageName = QObject::tr("%1%2%3").arg(prefix).arg(name[i]).arg(suffix);
    stageItem[i] = new StageMenuItem(imageName);
    stageItem[i]->setPos(position[i]);
    myItems.push_back(stageItem[i]);
  }

  if (type == 0)
    stageItem[5] = new StageMenuItem(":/images/stageitems/button_advance*.png");
  else
    stageItem[5] = new StageMenuItem(":/images/stageitems/button_normal*.png");
  stageItem[5]->setPos(position[5]);
  myItems.push_back(stageItem[5]);

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
  for (int i = 0; i < 5; ++i)
  {
    if (distanceOfTwoPoints(mousePos,
                            toScene(position[i].x(), position[i].y())) < 81)
    {
//      int *ballIndex = new int [61];
//      int *toBeIndex = new int [61];
//      getStageBallIndex(ballIndex, toBeIndex, i, 0);
//      RotatePuzzleGame *puzzleGame = new RotatePuzzleGame(ballIndex, toBeIndex);
      if (type == 0)
        emit giveControlTo(PuzzleGameInit::initRotatePuzzleGame(i, 2), false);
      else
        emit giveControlTo(PuzzleGameInit::initRotatePuzzleGame(i, 3), false);
    }
    if (distanceOfTwoPoints(mousePos,
                            toScene(position[5].x(), position[5].y())) < 81)
    {
      emit giveControlTo(new UniteStageMenuWidget(type ^ 1), true);
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
  addEffect(pixmap, width, height);
}

void UniteStageMenuWidget::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
  pixmap = QPixmap(width, height);
  pixmap.fill(Qt::black);
  QPainter *painter = new QPainter(&pixmap);
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           frameCount);

  painter->end();
  delete painter;
}

void UniteStageMenuWidget::addEffect(QPixmap& pixmap, int width, int height)
{

}

QPointF UniteStageMenuWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}

/////////////////////////////////////////////////////////////////////////////////

LockStageMenuWidget::LockStageMenuWidget(int stageType) :
    frameCount(0)
{
  type = stageType;
  stageItem = new StageMenuItem *[11];

  QString prefix = ":/images/stageitems/stage_img";
  QString suffix = "*.png";
  QString imageName = "";
  QString name[] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10"};
  for (int i = 0; i < 10; ++i)
  {
    imageName = QObject::tr("%1%2%3").arg(prefix).arg(name[i]).arg(suffix);
    //imageName.sprintf("%s%02d%s", prefix, i + 1, suffix);
    //qDebug() << imageName;
    stageItem[i] = new StageMenuItem(imageName);
    stageItem[i]->setPos(QPointF(0.13 + 0.07 + 0.15 * (i % 5), 0.15 + 0.05 + 0.45 * (i / 5) + 0.15 * (i % 2)));
    myItems.push_back(stageItem[i]);
  }

  if (type == 0)
    stageItem[10] = new StageMenuItem(":/images/stageitems/button_advance*.png");
  else
    stageItem[10] = new StageMenuItem(":/images/stageitems/button_normal*.png");
  stageItem[10]->setPos(QPointF(0.5, 0.5));
  myItems.push_back(stageItem[10]);

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
                            QPointF((0.13 + 0.07 + 0.15 * (i % 5)) * LOGICAL_WIDTH,
                                    (0.15 + 0.05 + 0.45 * (i / 5) + 0.15 * (i % 2)) * LOGICAL_HEIGHT)) < 81)
    {
//      int *ballIndex = new int [61];
//      int *toBeIndex = new int [61];
//      getStageBallIndex(ballIndex, toBeIndex, i, 0);
//      RotatePuzzleGame *puzzleGame = new RotatePuzzleGame(ballIndex, toBeIndex);
      if (type == 0)
        emit giveControlTo(PuzzleGameInit::initRotatePuzzleGame(i, 4), false);
      else
        emit giveControlTo(PuzzleGameInit::initRotatePuzzleGame(i, 5), false);
    }
  }
  if (distanceOfTwoPoints(mousePos,
                          QPointF(0.5 * LOGICAL_WIDTH,
                                  0.5 * LOGICAL_HEIGHT)) < 81)
  {
    emit giveControlTo(new LockStageMenuWidget(type ^ 1), true);
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
  addEffect(pixmap, width, height);
}

void LockStageMenuWidget::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
  pixmap = QPixmap(width, height);
  pixmap.fill(Qt::black);
  QPainter *painter = new QPainter(&pixmap);
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           frameCount);

  painter->end();
  delete painter;
}

void LockStageMenuWidget::addEffect(QPixmap& pixmap, int width, int height)
{

}

QPointF LockStageMenuWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}
