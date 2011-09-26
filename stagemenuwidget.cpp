#include "stagemenuwidget.h"

#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QFileInfo>
#include <QTextStream>
#include "basicpainter.h"
#include "stagemenuitems.h"
#include "rotatepuzzlegame.h"
#include "puzzlegameinit.h"
#include "gamerecord.h"

#include <QMessageBox>
#include <QDebug>

extern GameRecord gameRecord;

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

void ExchangeStageMenuWidget::makePixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height)
{
#ifdef USE_PIXMAP
      makeBasicPixmap(pixmap, width, height);
      addEffect(pixmap, width, height);
#else
      makeBasicPixmap(painter, width, height);
      addEffect(painter, width, height);
#endif
}

void ExchangeStageMenuWidget::makeBasicPixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height)
{
#ifdef USE_PIXMAP
  pixmap = QPixmap(width, height);

  // Fill the pixmap with black background
  pixmap.fill(Qt::black);

  // Get the painter
  QPainter *painter = new QPainter(&pixmap);
#else
  painter->fillRect(0,0,width,height,QColor(0,0,0));
#endif

  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           frameCount);

#ifdef USE_PIXMAP
  painter->end();
  delete painter;
#endif
}

void ExchangeStageMenuWidget::addEffect(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height)
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
  stageItem = new StageMenuItem *[6];
  suffix = "*.png";
  imageName = "";
  QString name[] = {"01", "02", "03", "04"};
  QPointF position[5];
  position[0] = QPointF(0.35, 0.35);
  position[1] = QPointF(0.5, 0.2);
  position[2] = QPointF(0.5, 0.5);
  position[3] = QPointF(0.65, 0.35);
  position[4] = QPointF(0.5, 0.8);

  // examine record
  if (!gameRecord.exists("exchange"))
  {
    gameRecord.createFile("exchange", 8);
    int *initialSteps = new int[8];
    for (int i = 0; i < 8; ++i)
      initialSteps[i] = -1;
    gameRecord.writeDataArr("exchange", initialSteps, 8);
  }

  int *leastSteps;
  int size;
  gameRecord.readDataArr("exchange", leastSteps, size);

  for (int i = 0; i < 4; ++i)
  {
    if (type == 0)
    {
      prefix = ":/images/stageitems/stage_img";
      if (i != 0 && leastSteps[i - 1] == -1)
        imageName = QObject::tr("%1%2%3").arg(prefix + "_locked").arg(name[i]).arg(suffix);
      else
        imageName = QObject::tr("%1%2%3").arg(prefix).arg(name[i]).arg(suffix);
    }
    else
    {
      prefix = ":/images/stageitems/stage_img_advanced";
      if (leastSteps[i] == -1)
        imageName = QObject::tr("%1%2%3").arg(prefix + "_locked").arg(name[i]).arg(suffix);
      else
        imageName = QObject::tr("%1%2%3").arg(prefix).arg(name[i]).arg(suffix);
    }
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

  // exit button
  stageItem[5] = new StageMenuItem(":/images/stageitems/button_exit*.png");
  stageItem[5]->setPos(QPointF(0.8, 0.8));
  myItems.push_back(stageItem[5]);

  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
}

void ExchangeStageMenuWidget::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  if (button == Qt::RightButton)
  {
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }

  int *leastSteps;
  int size;
  gameRecord.readDataArr("exchange", leastSteps, size);

  for (int i = 0; i < 4; ++i)
  {
    if (stageItem[i]->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
    { 
//      RotatePuzzleGame *puzzleGame = PuzzleGameInit::initRotatePuzzleGame(i, 0);
//      emit giveControlTo(puzzleGame, false);
      if (type == 0)
      {
        if (!(i != 0 && leastSteps[i - 1] == -1))
          emit giveControlTo(PuzzleGameInit::initRotatePuzzleGame(i, 0, leastSteps[i]), true);
        else
          return;
      }
      else
      {
        if (leastSteps[i] != -1)
          emit giveControlTo(PuzzleGameInit::initRotatePuzzleGame(i, 1, leastSteps[i + 4]), true);
        else
          return;
      }
      delete this;
      return;
    }
  }
  if (stageItem[4]->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
  {
    emit giveControlTo(new ExchangeStageMenuWidget(type ^ 1), true);
    delete this;
    return;
  }
  else if (stageItem[5]->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
  {
    emit giveControlTo(NULL, true);
    delete this;
    return;
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

void ExchangeStageMenuWidget::getForcus()
{
  t->start();
}

/////////////////////////////////////////////////////////////////////////////////

UniteStageMenuWidget::UniteStageMenuWidget(int stageType) :
    frameCount(0)
{
  type = stageType;
  stageItem = new StageMenuItem *[7];
  suffix = "*.png";
  imageName = "";

  // examine record
  if (!gameRecord.exists("unite"))
  {
    gameRecord.createFile("unite", 10);
    int *initialSteps = new int[10];
    for (int i = 0; i < 10; ++i)
      initialSteps[i] = -1;
    gameRecord.writeDataArr("unite", initialSteps, 10);
  }

  QString name[] = {"01", "02", "03", "04", "05"};
  QPointF position[6];
  position[0] = QPointF(0.35, 0.65);
  position[1] = QPointF(0.35, 0.35);
  position[2] = QPointF(0.5, 0.2);
  position[3] = QPointF(0.65, 0.35);
  position[4] = QPointF(0.65, 0.65);
  position[5] = QPointF(0.5, 0.8);

  int *leastSteps;
  int size;
  gameRecord.readDataArr("unite", leastSteps, size);

  for (int i = 0; i < 5; ++i)
  {
    if (type == 0)
    {
      prefix = ":/images/stageitems/stage_img";
      if (i != 0 && leastSteps[i - 1] == -1)
        imageName = QObject::tr("%1%2%3").arg(prefix + "_locked").arg(name[i]).arg(suffix);
      else
        imageName = QObject::tr("%1%2%3").arg(prefix).arg(name[i]).arg(suffix);
    }
    else
    {
      prefix = ":/images/stageitems/stage_img_advanced";
      if (leastSteps[i] == -1)
        imageName = QObject::tr("%1%2%3").arg(prefix + "_locked").arg(name[i]).arg(suffix);
      else
        imageName = QObject::tr("%1%2%3").arg(prefix).arg(name[i]).arg(suffix);
    }
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

  // exit button
  stageItem[6] = new StageMenuItem(":/images/stageitems/button_exit*.png");
  stageItem[6]->setPos(QPointF(0.8, 0.8));
  myItems.push_back(stageItem[6]);

  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
}

void UniteStageMenuWidget::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  if (button == Qt::RightButton)
  {
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }

  int *leastSteps;
  int size;
  gameRecord.readDataArr("unite", leastSteps, size);

  for (int i = 0; i < 5; ++i)
  {
    if (stageItem[i]->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
    {
//      int *ballIndex = new int [61];
//      int *toBeIndex = new int [61];
//      getStageBallIndex(ballIndex, toBeIndex, i, 0);
//      RotatePuzzleGame *puzzleGame = new RotatePuzzleGame(ballIndex, toBeIndex);
      if (type == 0)
      {
        if (!(i != 0 && leastSteps[i - 1] == -1))
          emit giveControlTo(PuzzleGameInit::initRotatePuzzleGame(i, 2, leastSteps[i]), true);
        else
          return;
      }
      else
      {
        if (leastSteps[i] != -1)
          emit giveControlTo(PuzzleGameInit::initRotatePuzzleGame(i, 3, leastSteps[i + 5]), true);
        else
          return;
      }
      delete this;
      return;
    }
    if (stageItem[5]->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
    {
      emit giveControlTo(new UniteStageMenuWidget(type ^ 1), true);
      delete this;
      return;
    }
    else if (stageItem[6]->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
    {
      emit giveControlTo(NULL, true);
      delete this;
      return;
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

void UniteStageMenuWidget::makePixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height)
{
#ifdef USE_PIXMAP
      makeBasicPixmap(pixmap, width, height);
      addEffect(pixmap, width, height);
#else
      makeBasicPixmap(painter, width, height);
      addEffect(painter, width, height);
#endif
}

void UniteStageMenuWidget::makeBasicPixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height)
{
#ifdef USE_PIXMAP
  pixmap = QPixmap(width, height);

  // Fill the pixmap with black background
  pixmap.fill(Qt::black);

  // Get the painter
  QPainter *painter = new QPainter(&pixmap);
#else
  painter->fillRect(0,0,width,height,QColor(0,0,0));
#endif

  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           frameCount);

#ifdef USE_PIXMAP
  painter->end();
  delete painter;
#endif
}

void UniteStageMenuWidget::addEffect(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height)
{

}

QPointF UniteStageMenuWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}

void UniteStageMenuWidget::getForcus()
{
  t->start();
}

/////////////////////////////////////////////////////////////////////////////////

LockStageMenuWidget::LockStageMenuWidget(int stageType) :
    frameCount(0)
{
  type = stageType;
  stageItem = new StageMenuItem *[12];

  QString prefix = ":/images/stageitems/stage_img";
  QString suffix = "*.png";
  QString imageName = "";

  // examine record
  if (!gameRecord.exists("lock"))
  {
    gameRecord.createFile("lock", 20);
    int *initialSteps = new int[20];
    for (int i = 0; i < 20; ++i)
      initialSteps[i] = -1;
    gameRecord.writeDataArr("lock", initialSteps, 20);
  }

  int *leastSteps;
  int size;
  gameRecord.readDataArr("lock", leastSteps, size);

  QString name[] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10"};
  for (int i = 0; i < 10; ++i)
  {
    if (type == 0)
    {
      prefix = ":/images/stageitems/stage_img";
      if (i != 0 && leastSteps[i - 1] == -1)
        imageName = QObject::tr("%1%2%3").arg(prefix + "_locked").arg(name[i]).arg(suffix);
      else
        imageName = QObject::tr("%1%2%3").arg(prefix).arg(name[i]).arg(suffix);
    }
    else
    {
      prefix = ":/images/stageitems/stage_img_advanced";
      if (leastSteps[i] == -1)
        imageName = QObject::tr("%1%2%3").arg(prefix + "_locked").arg(name[i]).arg(suffix);
      else
        imageName = QObject::tr("%1%2%3").arg(prefix).arg(name[i]).arg(suffix);
    }
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

  // exit button
  stageItem[11] = new StageMenuItem(":/images/stageitems/button_exit*.png");
  stageItem[11]->setPos(QPointF(0.8, 0.5));
  myItems.push_back(stageItem[11]);

  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
}
void LockStageMenuWidget::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  if (button == Qt::RightButton)
  {
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }

  int *leastSteps;
  int size;
  gameRecord.readDataArr("lock", leastSteps, size);

  for (int i = 0; i < 10; ++i)
  {
    if (stageItem[i]->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
    {
//      int *ballIndex = new int [61];
//      int *toBeIndex = new int [61];
//      getStageBallIndex(ballIndex, toBeIndex, i, 0);
//      RotatePuzzleGame *puzzleGame = new RotatePuzzleGame(ballIndex, toBeIndex);
      if (type == 0)
      {
        if (!(i != 0 && leastSteps[i - 1] == -1))
          emit giveControlTo(PuzzleGameInit::initRotatePuzzleGame(i, 4, leastSteps[i]), true);
        else
          return;
      }
      else
      {
        if (leastSteps[i] != -1)
          emit giveControlTo(PuzzleGameInit::initRotatePuzzleGame(i, 5, leastSteps[i + 10]), true);
        else
          return;
      }
      delete this;
      return;
    }
  }
  if (stageItem[10]->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
  {
    emit giveControlTo(new LockStageMenuWidget(type ^ 1), true);
    delete this;
    return;
  }
  else if (stageItem[11]->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
  {
    emit giveControlTo(NULL, true);
    delete this;
    return;
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

void LockStageMenuWidget::makePixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height)
{
#ifdef USE_PIXMAP
      makeBasicPixmap(pixmap, width, height);
      addEffect(pixmap, width, height);
#else
      makeBasicPixmap(painter, width, height);
      addEffect(painter, width, height);
#endif
}

void LockStageMenuWidget::makeBasicPixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height)
{
#ifdef USE_PIXMAP
  pixmap = QPixmap(width, height);

  // Fill the pixmap with black background
  pixmap.fill(Qt::black);

  // Get the painter
  QPainter *painter = new QPainter(&pixmap);
#else
  painter->fillRect(0,0,width,height,QColor(0,0,0));
#endif

  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           frameCount);

#ifdef USE_PIXMAP
  painter->end();
  delete painter;
#endif
}

void LockStageMenuWidget::addEffect(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height)
{

}

QPointF LockStageMenuWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}

void LockStageMenuWidget::getForcus()
{
  t->start();
}
