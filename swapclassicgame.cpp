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

SwapClassicGame::SwapClassicGame() :
    frameCount(0),
    noSolutionCount(0)
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
  exitToMainMenu->setPos(QPointF(0.1, 0.9));
  myItems.push_back(exitToMainMenu);

  hint = new HintItem();
  hint->setPos(QPointF(0.1, 0.7));
  myItems.push_back(hint);

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

void SwapClassicGame::showHint()
{
  int hintOnBoard = controller->hint();
  if (hintOnBoard >= 0)
    effectPainter->hintAt(gameboardInfo->positionOfIndex(hintOnBoard),
                          rule->gestureAllowed(AbstractRule::Rotate));
  else if (flame->getCurrent() > 0)
    effectPainter->hintAt(toScene(flame->getPos().x(),
                                  flame->getPos().y()), false);
  else if (star->getCurrent() > 0)
    effectPainter->hintAt(toScene(star->getPos().x(),
                                  star->getPos().y()), false);
  else
    gameOver();
}

void SwapClassicGame::gameOver()
{
  quitGame();
}

void SwapClassicGame::quitGame()
{
  emit giveControlTo(NULL, true);
  delete this;
}

void SwapClassicGame::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  currentPos = mousePos;
  if (distanceOfTwoPoints(mousePos,
                          toScene(flame->getPos().x(),
                                  flame->getPos().y())) < 50)
    itemAtPressPos = flame;
  else if (distanceOfTwoPoints(mousePos,
                               toScene(star->getPos().x(),
                                       star->getPos().y())) < 50)
    itemAtPressPos = star;
  else if (distanceOfTwoPoints(mousePos,
                               toScene(hint->getPos().x(),
                                       hint->getPos().y())) < 50)
    itemAtPressPos = hint;
  else if (distanceOfTwoPoints(mousePos,
                               toScene(exitToMainMenu->getPos().x(),
                                       exitToMainMenu->getPos().y())) < 50)
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
  effectPainter->clearUserMovingEliminationHints();
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
        effectPainter->flash();
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
        effectPainter->flash();
        star->minusOne();
      }
    }
    else if (itemAtPressPos == hint &&
             distanceOfTwoPoints(mousePos,
                                 toScene(hint->getPos().x(),
                                         hint->getPos().y())) < 50)
      showHint();
    else if (itemAtPressPos == exitToMainMenu &&
             distanceOfTwoPoints(mousePos,
                                 toScene(exitToMainMenu->getPos().x(),
                                         exitToMainMenu->getPos().y())) < 50)
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
  if (flame->getCurrent() > 0 ||
      star->getCurrent() > 0 ||
      controller->hint() >= 0)
    noSolutionCount = 0;
  else
    ++noSolutionCount;
  if (noSolutionCount > 100)
    gameOver();
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
    if (connectionCountOfThePosition > 0)
      effectPainter->highlightAt(i);
    if (connectionCountOfThePosition > 1)
    {
      if (connectionCountOfThePosition >= 2)
        effectPainter->flash();
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
    int size = connections.connections[i]->size();
    QPointF pos1(gameboardInfo->positionOfIndex(connections.connections[i]->at(0)));
    QPointF pos2(gameboardInfo->positionOfIndex(connections.connections[i]->at(size - 1)));
    effectPainter->wordsAt(QPointF((pos1.x() + pos2.x()) / 2,
                                   (pos1.y() + pos2.y()) / 2),
                           tr("%1").arg(size),
                           size);
    if (size >= 4)
      effectPainter->flash();
    if (size == 4)
      flame->addOne();
    if (size >= 5)
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
  nextStage->flame->setCurrent(flame->getCurrent());
  nextStage->star->setCurrent(star->getCurrent());
  emit giveControlTo(nextStage, true);
  delete this;
}
