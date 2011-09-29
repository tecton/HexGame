#include "rotatepuzzlegame.h"

#include <QPainter>
#include <QPixmap>
#include <QPointF>
#include <QTimer>
#include "ball.h"
#include "effectpainter.h"
#include "rules.h"
#include "sixtyonegameboardinfo.h"
#include "connections.h"
#include "corecontroller.h"
#include "gesturecontroller.h"
#include "basicpainter.h"
#include "puzzlegameinit.h"
#include "stagemenuwidget.h"
#include "gamemath.h"
#include "gamecommonitems.h"
#include "puzzlegamerecord.h"
#include "nextstagewidget.h"

RotatePuzzleGame::RotatePuzzleGame(int ballIndex[],
                                   int toBeIndex[],
                                   int gameIndex,
                                   int gameType,
                                   int minSteps) :
    direction(0),
    nextStageChoice(false),
    targetSize(1)
{
    rule = new RotatePuzzleGameRule();
    gameboardInfo = new SixtyOneGameBoardInfo();
    //RotatePuzzleGameSavedInfo savedInfo = readSaved();
    Ball **balls = new Ball *[gameboardInfo->totalBallCounts()];
    for (int i = 0; i < gameboardInfo->totalBallCounts(); ++i)
    {
      // init balls according to the index
      if (ballIndex[i] == -1)
      {
        balls[i] = new Ball((Ball::Color)6);
        balls[i]->setPos(gameboardInfo->positionOfIndex(i));
        balls[i]->setState(Ball::Stable);
        balls[i]->setLocked(true);
      }
      else if (ballIndex[i] != 0)
      {
        balls[i] = new Ball((Ball::Color)ballIndex[i]);
        balls[i]->setPos(gameboardInfo->positionOfIndex(i));
        balls[i]->setState(Ball::Stable);
      }
      else
      {
        balls[i] = NULL;
      }
    }
    completeIndex = new int[gameboardInfo->totalBallCounts()];
    for (int i = 0; i < gameboardInfo->totalBallCounts(); ++i)
      completeIndex[i] = toBeIndex[i];

    index = gameIndex;
    type = gameType;

    // init two hint information
    currentSteps = new IntegerItem();
    currentSteps->setValue(0);
    currentSteps->setHint("Current moved steps:");
    currentSteps->setPos(QPointF(0.15, 0.5));
    minimalSteps = new IntegerItem();
    minimalSteps->setHint(" Least moved steps: ");
    minimalSteps->setValue((minSteps != -1) ? minSteps : 99);
    minimalSteps->setPos(QPointF(0.15, 0.65));
    exitItem = new ButtonItem("Exit");
    exitItem->setPos(QPointF(0.15,0.85));
    // collect them
    myItems.push_back(currentSteps);
    myItems.push_back(minimalSteps);
    myItems.push_back(exitItem);

    controller = new CoreController(rule, gameboardInfo, balls);

    //  Create the effect painter
    effectPainter = new EffectPainter(gameboardInfo);

    // Create the gesture controller
    gestureController = new GestureController(rule,
                                              gameboardInfo,
                                              controller,
                                              effectPainter);

    t = new QTimer();
    t->setInterval(75);
    connect(t, SIGNAL(timeout()), this, SLOT(advance()));
    connect(controller, SIGNAL(goodMove()), this, SLOT(successMoved()));
}

void RotatePuzzleGame::makePixmap(
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

RotatePuzzleGame::~RotatePuzzleGame()
{
    t->stop();
    delete t;
    // delete the collected items
    for (int i = 0;i < myItems.size();++i)
      delete myItems[i];
    delete controller;
    delete gameboardInfo;
    delete rule;
    delete gestureController;
    delete effectPainter;
}


void RotatePuzzleGame::makeBasicPixmap(
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
#endif

    Ball **balls = controller->balls;
    BasicPainter::paintBackGround(BasicPainter::Game61,
                                  painter,
                                  width,
                                  height,
                                  frameCount);
    BasicPainter::paintBasicBalls(gameboardInfo,
                                  balls,
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
    if (targetSize != 1)
      BasicPainter::darken(painter,
                           width,
                           height);
    BasicPainter::paintPuzzleGameBalls(gameboardInfo,
                                       completeIndex,
                                       gameboardInfo->totalBallCounts(),
                                       painter,
                                       width * 1.0 / gameboardInfo->width(),
                                       height * 1.0 / gameboardInfo->height(),
                                       frameCount,
                                       targetSize);
#ifdef USE_PIXMAP
    painter->end();
    delete painter;
#endif
}

void RotatePuzzleGame::addEffect(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height)
{
#ifdef USE_PIXMAP
    QPainter *painter = new QPainter(&pixmap);
#endif
    effectPainter->paint(painter,
                         width * 1.0 / gameboardInfo->width(),
                         height * 1.0 / gameboardInfo->height());
#ifdef USE_PIXMAP
    painter->end();
    delete painter;
#endif
}

QPointF RotatePuzzleGame::toScene(double xRate, double yRate)
{
    return QPointF(xRate * gameboardInfo->width(),
                   yRate * gameboardInfo->height());
}

void RotatePuzzleGame::quitGame()
{
  // create a new widget and give control to it
  switch (type)
  {
  case 0:
    emit giveControlTo(new ExchangeStageMenuWidget(0), true);
    break;
  case 1:
    emit giveControlTo(new ExchangeStageMenuWidget(1), true);
    break;
  case 2:
    emit giveControlTo(new UniteStageMenuWidget(0), true);
    break;
  case 3:
    emit giveControlTo(new UniteStageMenuWidget(1), true);
    break;
  case 4:
    emit giveControlTo(new LockStageMenuWidget(0), true);
    break;
  case 5:
    emit giveControlTo(new LockStageMenuWidget(1), true);
    break;
  }
  delete this;
}

void RotatePuzzleGame::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  if (button == Qt::RightButton)
  {
    quitGame();
    return;
  }

  if (targetSize == 1 && direction == 0)
  {
    if (exitItem->in(mousePos,
                     gameboardInfo->width(),
                     gameboardInfo->height()))
    {
      quitGame();
      return;
    }
    gestureController->dealPressed(mousePos);
  }
  if (targetSize == 1 &&
      direction == 0 &&
      distanceOfTwoPoints(mousePos,
                          toScene(0.2, 0.2)) < 100)
    direction = 1;
  else if (targetSize != 1 || direction != 0)
    direction = 2;
}

void RotatePuzzleGame::dealMoved(QPointF mousePos, Qt::MouseButton button)
{
  if (targetSize == 1 && direction == 0)
    gestureController->dealMoved(mousePos);
}

void RotatePuzzleGame::dealReleased(QPointF mousePos, Qt::MouseButton button)
{
  if (targetSize == 1 && direction == 0)
    gestureController->dealReleased(mousePos);
}

void RotatePuzzleGame::getForcus()
{
  t->start();
}

void RotatePuzzleGame::successMoved()
{
  currentSteps->setValue(currentSteps->getValue() + 1);
}

void RotatePuzzleGame::advance()
{
  switch (direction)
  {
  case 1:
    if (targetSize < 6)
      ++targetSize;
    else
      direction = 0;
    break;
  case 2:
    if (targetSize > 1)
      --targetSize;
    else
      direction = 0;
    break;
  }

    ++frameCount;
    controller->advance();

    Ball **balls = controller->balls;
  //    delete balls;
    int i;
    for (i = 0; i < gameboardInfo->totalBallCounts(); ++i)
    {
      if (!((balls[i] == NULL && completeIndex[i] == 0)
            || balls[i]->getColor() == (Ball::Color)completeIndex[i]
            || (balls[i]->getLocked() && (completeIndex[i] == -1))))
        break;
    }
    if (i == gameboardInfo->totalBallCounts() && !nextStageChoice)
    {
      // set the bool to true
      nextStageChoice = true;

      // Create the next stage widget
      NextStageWidget *w = new NextStageWidget();

      // Connect
      connect(w, SIGNAL(confirm()), this, SLOT(nextStage()));

      // Give control to it
      emit giveControlTo(w, true);
    }
}

void RotatePuzzleGame::nextStage()
{
  int stageCount = PuzzleGameRecord::totalStages(type);
  if (index + 1 < stageCount)
  {
    RotatePuzzleGame* nextStage =
        PuzzleGameInit::initRotatePuzzleGame(index + 1, type);
    PuzzleGameRecord::testLeastSteps(type, index, currentSteps->getValue());
    emit giveControlTo(nextStage, true);
    delete this;
    return;
  }
  else
  {
    PuzzleGameRecord::testLeastSteps(type, index, currentSteps->getValue());
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }
}
