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
#include "puzzlegamerecord.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

QPointF AbstractStageMenuWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}
void AbstractStageMenuWidget::dealPressed(QPointF mousePos,
                                          Qt::MouseButton button)
{
  if (button == Qt::RightButton)
  {
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }

  for (int i = 0; i < stageCount; ++i)
  {
    if (!(stageItem[i]->getLocked()) &&
        stageItem[i]->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
    {
      emit giveControlTo(PuzzleGameInit::initRotatePuzzleGame(i, getType()), true);
      delete this;
      return;
    }
  }
  if (stageItem[stageCount]->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
  {
    emit giveControlTo(switchType(type), true);
    delete this;
    return;
  }
  else if (stageItem[stageCount + 1]->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
  {
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }
}

void AbstractStageMenuWidget::dealMoved(QPointF mousePos,
                                        Qt::MouseButton button){}

void AbstractStageMenuWidget::dealReleased(QPointF mousePos,
                                           Qt::MouseButton button){}

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

ExchangeStageMenuWidget::ExchangeStageMenuWidget(int stageType) :
  frameCount(0)
{
  type = stageType;
  stageCount = PuzzleGameRecord::totalStages(getType());
  stageItem = new StageMenuItem *[stageCount + 2];
  QPointF position[5];
  position[0] = QPointF(0.35, 0.35);
  position[1] = QPointF(0.5, 0.2);
  position[2] = QPointF(0.5, 0.5);
  position[3] = QPointF(0.65, 0.35);
  position[4] = QPointF(0.5, 0.8);

  int *leastSteps;
  int size;
  PuzzleGameRecord::minSteps(type, leastSteps, size);

  for (int i = 0; i < stageCount; ++i)
  {
    stageItem[i] = new StageMenuItem(i,
                                     i != 0 && leastSteps[i - 1] == -1,
                                     type == 1);
    stageItem[i]->setPos(position[i]);
    myItems.push_back(stageItem[i]);
  }

  delete [] leastSteps;

  // advance/normal button
  stageItem[stageCount] = new StageMenuItem(type == 1);
  stageItem[stageCount]->setPos(position[stageCount]);
  myItems.push_back(stageItem[stageCount]);

  // exit button
  stageItem[stageCount + 1] = new StageMenuItem();
  stageItem[stageCount + 1]->setPos(QPointF(0.8, 0.8));
  myItems.push_back(stageItem[stageCount + 1]);

  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
}

void ExchangeStageMenuWidget::advance()
{
  ++frameCount;
  //effectPainter->advance();
}

ExchangeStageMenuWidget::~ExchangeStageMenuWidget()
{
  // Stop the timer
  t->stop();
  // Release the space
  delete t;
  delete [] stageItem;
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
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
  stageCount = PuzzleGameRecord::totalStages(getType());
  stageItem = new StageMenuItem *[stageCount + 2];

  QPointF position[6];
  position[0] = QPointF(0.35, 0.65);
  position[1] = QPointF(0.35, 0.35);
  position[2] = QPointF(0.5, 0.2);
  position[3] = QPointF(0.65, 0.35);
  position[4] = QPointF(0.65, 0.65);
  position[5] = QPointF(0.5, 0.8);

  int *leastSteps;
  int size;
  PuzzleGameRecord::minSteps(type, leastSteps, size);

  for (int i = 0; i < stageCount; ++i)
  {
    stageItem[i] = new StageMenuItem(i,
                                     i != 0 && leastSteps[i - 1] == -1,
                                     type == 1);
    stageItem[i]->setPos(position[i]);
    myItems.push_back(stageItem[i]);
  }

  delete [] leastSteps;

  // advance/normal button
  stageItem[stageCount] = new StageMenuItem(type == 1);
  stageItem[stageCount]->setPos(position[stageCount]);
  myItems.push_back(stageItem[stageCount]);

  // exit button
  stageItem[stageCount + 1] = new StageMenuItem();
  stageItem[stageCount + 1]->setPos(QPointF(0.8, 0.8));
  myItems.push_back(stageItem[stageCount + 1]);

  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
}

void UniteStageMenuWidget::advance()
{
  ++frameCount;
  //effectPainter->advance();
}

UniteStageMenuWidget::~UniteStageMenuWidget()
{
  // Stop the timer
  t->stop();
  // Release the space
  delete t;
  delete [] stageItem;
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
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

void UniteStageMenuWidget::getForcus()
{
  t->start();
}

/////////////////////////////////////////////////////////////////////////////////

LockStageMenuWidget::LockStageMenuWidget(int stageType) :
    frameCount(0)
{
  type = stageType;
  stageCount = PuzzleGameRecord::totalStages(getType());
  stageItem = new StageMenuItem *[stageCount + 2];

  int *leastSteps;
  int size;
  PuzzleGameRecord::minSteps(type, leastSteps, size);

  for (int i = 0; i < stageCount; ++i)
  {
    stageItem[i] = new StageMenuItem(i,
                                     i != 0 && leastSteps[i - 1] == -1,
                                     type == 1);
    stageItem[i]->setPos(QPointF(0.13 + 0.07 + 0.15 * (i % 5), 0.15 + 0.05 + 0.45 * (i / 5) + 0.15 * (i % 2)));
    myItems.push_back(stageItem[i]);
  }

  delete [] leastSteps;

  // advance/normal button
  stageItem[stageCount] = new StageMenuItem(type == 1);
  stageItem[stageCount]->setPos(QPointF(0.5, 0.5));
  myItems.push_back(stageItem[stageCount]);

  // exit button
  stageItem[stageCount + 1] = new StageMenuItem();
  stageItem[stageCount + 1]->setPos(QPointF(0.8, 0.5));
  myItems.push_back(stageItem[stageCount + 1]);

  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
}

void LockStageMenuWidget::advance()
{
  ++frameCount;
  //effectPainter->advance();
}

LockStageMenuWidget::~LockStageMenuWidget()
{
  // Stop the timer
  t->stop();
  // Release the space
  delete t;
  delete [] stageItem;
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
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

void LockStageMenuWidget::getForcus()
{
  t->start();
}
