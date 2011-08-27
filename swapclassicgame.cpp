#include "swapclassicgame.h"

#include <QPainter>
#include <QPixmap>
#include <QPointF>
#include <QTimer>
#include "effectpainter.h"
#include "rules.h"
//#include "sixtyonegameboardinfo.h"
#include "connections.h"
#include "corecontroller.h"
#include "gesturecontroller.h"
#include "basicpainter.h"
#include "gamecommonitems.h"
#include "gamemath.h"
#include "thirtysevengameboardinfo.h"

#define LOGICAL_WIDTH  800
#define LOGICAL_HEIGHT 500

SwapClassicGame::SwapClassicGame()
{
  rule = new SwapClassicGameRule();//SwapClassicGameRule();
  gameboardInfo = new ThirtySevenGameBoardInfo();
  //SwapClassicGameSavedInfo savedInfo = readSaved();
  controller = new CoreController(rule, gameboardInfo, NULL);
  controller->fillAllBlanks();
  gestureController = new GestureController(rule, gameboardInfo, controller);

  effectPainter = new EffectPainter(gameboardInfo);

  progressBar = new VerticalProgressBarItem();
  progressBar->setPos(QPointF(0.1, 0.1));
  myItems.push_back(progressBar);

  flame = new FlameItem();
  flame->setPos(QPointF(0.1, 0.3));
  myItems.push_back(flame);

  star = new StarItem();
  star->setPos(QPointF(0.1, 0.5));
  myItems.push_back(star);

  exitToMainMenu = new ExitToMainMenuItem();
  exitToMainMenu->setPos(QPointF(0.1, 0.7));
  myItems.push_back(exitToMainMenu);

  itemAtPressPos = NULL;

  connect(controller,
          SIGNAL(stableEliminateTested(Connections)),
          this,
          SLOT(dealStableEliminate(Connections)));
  connect(controller,
          SIGNAL(userMovingEliminateTested(Connections)),
          this,
          SLOT(dealUserMovingEliminate(Connections)));

  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
  t->start();
}

void SwapClassicGame::makePixmap(QPixmap& pixmap, int width, int height)
{
  makeBasicPixmap(pixmap, width, height);
  addEffect(pixmap, width, height);
}

SwapClassicGame::~SwapClassicGame()
{
  t->stop();
  delete t;
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
  delete controller;
  delete gameboardInfo;
  delete rule;
  delete gestureController;
  delete effectPainter;
}

//void SwapClassicGame::init() //
//}

void SwapClassicGame::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
  pixmap = QPixmap(width, height);
  pixmap.fill(Qt::black);
  QPainter *painter = new QPainter(&pixmap);
  Ball **balls = controller->balls;
  BasicPainter::paintBasicBalls(balls,
                                gameboardInfo->totalBallCounts(),
                                painter,
                                width * 1.0 / gameboardInfo->width(),
                                height * 1.0 / gameboardInfo->height(),
                                frameCount);
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           frameCount);
  painter->end();
  delete painter;
}

void SwapClassicGame::addEffect(QPixmap& pixmap, int width, int height)
{
  QPainter *painter = new QPainter(&pixmap);
  QPointF pos = currentPos;
  pos.setX(currentPos.x() * width / gameboardInfo->width());
  pos.setY(currentPos.y() * height / gameboardInfo->height());

  effectPainter->clearBonusEliminationHints();
  if (itemAtPressPos != NULL)
  {
    if (itemAtPressPos == flame && flame->notEmpty())
    {
      flame->paintLocatingIcon(painter, pos, frameCount);
      int index = gameboardInfo->indexOfMousePosition(currentPos);
      flame->paintInfluencedArea(index, gameboardInfo, effectPainter, frameCount);
    }
    else if (itemAtPressPos == star && star->notEmpty())
    {
      star->paintLocatingIcon(painter, pos, frameCount);
      int index = gameboardInfo->indexOfMousePosition(currentPos);
      star->paintInfluencedArea(index, gameboardInfo, effectPainter, frameCount);
    }
  }

  effectPainter->paint(painter,
                       width * 1.0 / gameboardInfo->width(),
                       height * 1.0 / gameboardInfo->height());


  painter->end();
  delete painter;
  effectPainter->advance();
}

QPointF SwapClassicGame::toScene(double xRate, double yRate)
{
  return QPointF(xRate * gameboardInfo->width(),
                 yRate * gameboardInfo->height());
}

//SwapClassicGameSavedInfo SwapClassicGame::readSaved()
//{

//}
void SwapClassicGame::quitGame()
{
  emit giveControlTo(NULL, true);
  delete this;
}

void SwapClassicGame::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  currentPos = mousePos;
  if (distanceOfTwoPoints(mousePos,
                          QPointF(0.1 * LOGICAL_WIDTH,
                                  0.3 * LOGICAL_HEIGHT)) < 50)
    itemAtPressPos = flame;
  else if (distanceOfTwoPoints(mousePos,
                               QPointF(0.1 * LOGICAL_WIDTH,
                                       0.5 * LOGICAL_HEIGHT)) < 50)
    itemAtPressPos = star;
  else if (distanceOfTwoPoints(mousePos,
                               QPointF(0.1 * LOGICAL_WIDTH,
                                       0.7 * LOGICAL_HEIGHT)) < 50)
    itemAtPressPos = exitToMainMenu;
  else
    itemAtPressPos = NULL;


  if (button == Qt::RightButton)
  {
    quitGame();
    return;
  }
  gestureController->dealPressed(mousePos);
}

void SwapClassicGame::dealMoved(QPointF mousePos, Qt::MouseButton button)
{
  currentPos = mousePos;

  gestureController->dealMoved(mousePos);
}

void SwapClassicGame::dealReleased(QPointF mousePos, Qt::MouseButton button)
{
  if (itemAtPressPos != NULL)
  {
    if (itemAtPressPos == flame && flame->notEmpty())
    {
      int index = gameboardInfo->indexOfMousePosition(mousePos);
      if (index != -1)
      {
        controller->flameAt(index);
        effectPainter->explodeAt(index);
        flame->minusOne();
      }
    }
    else if (itemAtPressPos == star && star->notEmpty())
    {
      int index = gameboardInfo->indexOfMousePosition(mousePos);
      if (index != -1)
      {
        controller->starAt(index);
        effectPainter->lightningAt(index/*, directions*/);
        star->minusOne();
      }
    }
    else if (itemAtPressPos == exitToMainMenu)
      quitGame();
  }

  effectPainter->clearUserMovingEliminationHints();
  itemAtPressPos = NULL;
  gestureController->dealReleased(mousePos);
}

void SwapClassicGame::advance()
{
  if (progressBar->getCurrent() >= progressBar->getMax())
  {
    nextStage();
    return;
  }
  ++frameCount;
  frameCount = frameCount % 32;
  controller->advance();
//  effectPainter->advance();
}

void SwapClassicGame::dealStableEliminate(Connections connections)
{
  int pointsToAdd = 0;
  for (int i = 0;i < gameboardInfo->totalBallCounts();++i)
  {
    QVector<QVector<int> *>& connect = connections.connectionsOfIndex[i];
    int connectionCountOfThePosition = 0;
    for (int j = 0;j < 10;++j)
    {
      if (j == 3 || connect[j] == NULL)
        continue;
      ++connectionCountOfThePosition;
    }
    if (connectionCountOfThePosition > 1)
    {
      effectPainter->highlightAt(i);
      if (connectionCountOfThePosition == 2)
        flame->addOne();
      if (connectionCountOfThePosition >= 3)
        star->addOne();
      // TODO:BLABLABLA

    }
    if (connectionCountOfThePosition > 0)
      ++pointsToAdd;
  }
  for (int i = 0;i < connections.connections.size();++i)
  {
    if (connections.connections[i]->size() == 4)
      flame->addOne();
    if (connections.connections[i]->size() >= 5)
      star->addOne();
  }
  progressBar->setCurrent(progressBar->getCurrent() + pointsToAdd);
}

void SwapClassicGame::dealUserMovingEliminate(Connections connections)
{
  effectPainter->clearUserMovingEliminationHints();
  for (int i = 0;i < connections.connections.size();++i)
  {
    for (int j = 0;j < connections.connections[i]->size();++j)
      effectPainter->userMovingEliminationHintAt(connections.connections[i]->at(j));
    // TODO:BLABLABLA
  }
}

void SwapClassicGame::nextStage()
{
  SwapClassicGame *nextStage = new SwapClassicGame();
  nextStage->progressBar->setMin(progressBar->getMax());
  nextStage->progressBar->setMax(progressBar->getMax() * 2);
  nextStage->progressBar->setCurrent(progressBar->getCurrent());
  emit giveControlTo(nextStage, true);
  delete this;
}
