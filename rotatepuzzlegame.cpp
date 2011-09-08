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
#include "gamerecord.h"
#include "gamemath.h"
#include "gamecommonitems.h"

extern GameRecord gameRecord;

RotatePuzzleGame::RotatePuzzleGame(int ballIndex[], int toBeIndex[],
                                   int gameIndex, int gameType, int minSteps)
{
    rule = new RotatePuzzleGameRule();
    gameboardInfo = new SixtyOneGameBoardInfo();
    //RotatePuzzleGameSavedInfo savedInfo = readSaved();
    Ball **balls = new Ball *[gameboardInfo->totalBallCounts()];
    toBeShapeBalls = new Ball *[gameboardInfo->totalBallCounts()];
    for (int i = 0; i < gameboardInfo->totalBallCounts(); ++i)
    {
      if (ballIndex[i] == -1)
      {
        balls[i] = new Ball((Ball::Color)6);
        toBeShapeBalls[i] = new Ball((Ball::Color)6);
        balls[i]->setPos(gameboardInfo->positionOfIndex(i));
        toBeShapeBalls[i]->setPos(gameboardInfo->positionOfIndex(i));
        balls[i]->setLocked(true);
      }
      else if (ballIndex[i] != 0)
      {
        balls[i] = new Ball((Ball::Color)ballIndex[i]);
        balls[i]->setPos(gameboardInfo->positionOfIndex(i));
        toBeShapeBalls[i] = new Ball((Ball::Color)toBeIndex[i]);
        toBeShapeBalls[i]->setPos(gameboardInfo->positionOfIndex(i));
      }
      else
      {
        balls[i] = NULL;
        toBeShapeBalls[i] = NULL;
      }
    }
    completeIndex = new int[gameboardInfo->totalBallCounts()];
    for (int i = 0; i < gameboardInfo->totalBallCounts(); ++i)
      completeIndex[i] = toBeIndex[i];

    index = gameIndex;
    type = gameType;

    currentSteps = new IntegerItem();
    currentSteps->setValue(0);
    currentSteps->setHint("Current moved steps:");
    currentSteps->setPos(QPointF(0.15, 0.5));
    minimalSteps = new IntegerItem();
    minimalSteps->setHint(" Least moved steps: ");
    minimalSteps->setValue((minSteps != -1) ? minSteps : 99);

    minimalSteps->setPos(QPointF(0.15, 0.65));
    exitItem = new ExitItem();
    exitItem->setPos(QPointF(0.15,0.85));
    myItems.push_back(currentSteps);
    myItems.push_back(minimalSteps);
    myItems.push_back(exitItem);

    controller = new CoreController(rule, gameboardInfo, balls);
    gestureController = new GestureController(rule, gameboardInfo, controller);

    effectPainter = new EffectPainter(gameboardInfo);

    t = new QTimer();
    t->setInterval(75);
    connect(t, SIGNAL(timeout()), this, SLOT(advance()));
    connect(controller, SIGNAL(goodMove()), this, SLOT(successMoved()));
}

void RotatePuzzleGame::makePixmap(QPixmap& pixmap, int width, int height)
{
    makeBasicPixmap(pixmap, width, height);
    addEffect(pixmap, width, height);
}

RotatePuzzleGame::~RotatePuzzleGame()
{
    t->stop();
    for (int i = 0;i < gameboardInfo->totalBallCounts();++i)
      if (toBeShapeBalls[i])
        delete toBeShapeBalls[i];
    delete [] toBeShapeBalls;
    delete t;
    delete controller;
    delete gameboardInfo;
    delete rule;
    delete gestureController;
    delete effectPainter;
}


void RotatePuzzleGame::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
    pixmap = QPixmap(width, height);
    pixmap.fill(Qt::black);
    QPainter *painter = new QPainter(&pixmap);
    Ball **balls = controller->balls;
    Ball **targetBalls = toBeShapeBalls;
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
    BasicPainter::paintPuzzleGameBalls(targetBalls,
                                       completeIndex,
                                       gameboardInfo->totalBallCounts(),
                                       painter,
                                       width * 0.4 / gameboardInfo->width(),
                                       height * 0.4 / gameboardInfo->height(),
                                       frameCount);
    BasicPainter::paintItems(painter,
                             myItems,
                             width,
                             height,
                             frameCount);
    painter->end();
    delete painter;
}

void RotatePuzzleGame::addEffect(QPixmap& pixmap, int width, int height)
{
    QPainter *painter = new QPainter(&pixmap);
    effectPainter->paint(painter,
                         width * 1.0 / gameboardInfo->width(),
                         height * 1.0 / gameboardInfo->height());
    painter->end();
    delete painter;
}

QPointF RotatePuzzleGame::toScene(double xRate, double yRate)
{
    return QPointF(xRate * gameboardInfo->width(),
                   yRate * gameboardInfo->height());
}

//RotatePuzzleGameSavedInfo RotatePuzzleGame::readSaved()
//{

//}
void RotatePuzzleGame::quitGame()
{
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
    if (distanceOfTwoPoints(mousePos,
                            toScene(exitItem->getPos().x(),
                                    exitItem->getPos().y())) < 50)
    {
      quitGame();
      return;
    }
    gestureController->dealPressed(mousePos);
}

void RotatePuzzleGame::dealMoved(QPointF mousePos, Qt::MouseButton button)
{
    gestureController->dealMoved(mousePos);
}

void RotatePuzzleGame::dealReleased(QPointF mousePos, Qt::MouseButton button)
{
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
    if (i == gameboardInfo->totalBallCounts())
    {
      int lastStageIndex[] = {4, 4, 5, 5, 10, 10};
      int offset[] = {0, 4, 0, 5, 0, 10};
      QString fileName[] = {"exchange", "exchange", "unite", "unite",
                           "lock", "lock"};
      if (index != lastStageIndex[type])
      {
        RotatePuzzleGame* nextStage = PuzzleGameInit::initRotatePuzzleGame(index + 1,
                                                                           type,
                                                                           gameRecord.readData(fileName[type], index + 1 + offset[type]));
        gameRecord.writeData(fileName[type], index + offset[type], currentSteps->getValue());
        emit giveControlTo(nextStage, true);
      }
      else
      {
        gameRecord.writeData(fileName[type], index + offset[type], currentSteps->getValue());
        emit giveControlTo(NULL, true);
      }
//      if (index != 5)
//      {
//        RotatePuzzleGame* nextStage = PuzzleGameInit::initRotatePuzzleGame(index + 1,
//                                                                           type);
//        emit giveControlTo(nextStage, true);
//      }
//      else if (index == 5)
//        emit giveControlTo(NULL, true);
      delete this;
    }
    //  effectPainter->advance();
}
