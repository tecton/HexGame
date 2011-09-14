#include "endlessgamewidget.h"

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
#include "othergameinit.h"
#include "resetwidget.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

EndlessGameWidget::EndlessGameWidget(AbstractRule::Gesture gesture) :
    frameCount(0)
{
  // Create the rule
  if (gesture == AbstractRule::Swap)
    rule = new SwapEndlessGameRule();
  else
    rule = new RotateEndlessGameRule();

  // Create the gameboard info
  gameboardInfo = new ThirtySevenGameBoardInfo();

  // Load the game
  OtherGameRecord *record = OtherGameInit::loadOtherGame(getIndex());

  // Create the controller
  controller = new CoreController(rule, gameboardInfo, record->balls);

  // Move the balls to the correct position
  // and avoid elimination at the beginning
  controller->fillAllBlanks();
  for (int i = 0;i < 100;++i)
    controller->advance();

  // Create the gesture controller
  gestureController = new GestureController(rule, gameboardInfo, controller);

  //  Create the effect painter
  effectPainter = new EffectPainter(gameboardInfo);

  // Create the items and initialize them
  hightestScore = new IntegerItem();
  hightestScore->setPos(QPointF(0.1, 0.1));
  hightestScore->setValue(record->highestScore);
  hightestScore->setHint("Highest Score");
  myItems.push_back(hightestScore);

  currentLevel = new IntegerItem();
  currentLevel->setPos(QPointF(0.1, 0.25));
  currentLevel->setValue(record->currentLevel);
  currentLevel->setHint("Current Level");
  myItems.push_back(currentLevel);

  progressBar = new VerticalProgressBarItem();
  progressBar->setPos(QPointF(0.25, 0.5));
  progressBar->setCurrent(record->currentScore);
  progressBar->setMin(record->minScore);
  progressBar->setMax(record->maxScore);
  myItems.push_back(progressBar);

  flame = new FlameItem();
  flame->setPos(QPointF(0.1, 0.4));
  flame->setCurrent(record->flame);
  myItems.push_back(flame);

  star = new StarItem();
  star->setPos(QPointF(0.1, 0.55));
  star->setCurrent(record->star);
  myItems.push_back(star);

  hint = new HintItem();
  hint->setPos(QPointF(0.1, 0.7));
  myItems.push_back(hint);

  resetItem = new ResetItem();
  resetItem->setPos(QPointF(0.1, 0.8));
  myItems.push_back(resetItem);

  exitItem = new ExitItem();
  exitItem->setPos(QPointF(0.1, 0.9));
  myItems.push_back(exitItem);

  // No items was chosen
  itemAtPressPos = NULL;

  // Release the space
  delete record;

  // Connect signals and slots
  connect(controller,
          SIGNAL(stableEliminateTested(Connections)),
          this,
          SLOT(dealStableEliminate(Connections)));
  connect(controller,
          SIGNAL(userMovingEliminateTested(Connections)),
          this,
          SLOT(dealUserMovingEliminate(Connections)));
  connect(controller,
          SIGNAL(eliminated(int)),
          this,
          SLOT(eliminated(int)));

  // Create the timer and connect signals and slots
  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
}

void EndlessGameWidget::makePixmap(QPixmap& pixmap, int width, int height)
{
  makeBasicPixmap(pixmap, width, height);
  addEffect(pixmap, width, height);
}

EndlessGameWidget::~EndlessGameWidget()
{
  // Stop the timer
  t->stop();
  // Release the space
  delete t;
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
  delete controller;
  delete gameboardInfo;
  delete rule;
  delete gestureController;
  delete effectPainter;
}

//void EndlessGameWidget::init() //
//}

void EndlessGameWidget::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
  pixmap = QPixmap(width, height);

  // Fill the pixmap with black background
  pixmap.fill(Qt::black);

  // Get the painter
  QPainter *painter = new QPainter(&pixmap);

  // Get the balls
  Ball **balls = controller->balls;

  // Paint the background
  BasicPainter::paintBackGround(BasicPainter::Game37,
                                painter,
                                width,
                                height,
                                frameCount);

  // Paint the basic balls
  BasicPainter::paintBasicBalls(gameboardInfo,
                                balls,
                                gameboardInfo->totalBallCounts(),
                                painter,
                                width * 1.0 / gameboardInfo->width(),
                                height * 1.0 / gameboardInfo->height(),
                                frameCount);

  // Paint the items
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           frameCount);

  // End the paint and release the space
  painter->end();
  delete painter;
}

void EndlessGameWidget::addEffect(QPixmap& pixmap, int width, int height)
{
  // Get the painter
  QPainter *painter = new QPainter(&pixmap);

  // Calculte the bonus hint and show it
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

  // Paint the effects
  effectPainter->paint(painter,
                       width * 1.0 / gameboardInfo->width(),
                       height * 1.0 / gameboardInfo->height());

  // End the paint and release the space
  painter->end();
  delete painter;

  // Advance the effect painter
  effectPainter->advance();
}

QPointF EndlessGameWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * gameboardInfo->width(),
                 yRate * gameboardInfo->height());
}

void EndlessGameWidget::showHint()
{
  // Get the index of the hint
  int hintOnBoard = controller->hint();

  // Show the hint
  effectPainter->hintAt(gameboardInfo->centerPositionOfIndex(hintOnBoard),
                        rule->gestureAllowed(AbstractRule::Rotate));
}

void EndlessGameWidget::quitGame()
{
  // Make the record and save it
  OtherGameRecord record;
  record.currentLevel = currentLevel->getValue();
  record.minScore = progressBar->getMin();
  record.currentScore = progressBar->getCurrent();
  record.maxScore = progressBar->getMax();
  record.flame = flame->getCurrent();
  record.star = star->getCurrent();
  record.balls = controller->balls;
  OtherGameInit::saveOtherGame(&record,
                               getIndex(),
                               gameboardInfo->totalBallCounts());

  emit giveControlTo(NULL, true);
  delete this;
}

void EndlessGameWidget::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  // Choose the correct item at press position
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
                               toScene(resetItem->getPos().x(),
                                       resetItem->getPos().y())) < 50)
    itemAtPressPos = resetItem;
  else if (distanceOfTwoPoints(mousePos,
                               toScene(exitItem->getPos().x(),
                                       exitItem->getPos().y())) < 50)
    itemAtPressPos = exitItem;
  else
    itemAtPressPos = NULL;

  // Quit if it's a right button
  // May be abandoned later
  if (button == Qt::RightButton)
  {
    quitGame();
    return;
  }

  // Let the gesture controller to deal the press event
  gestureController->dealPressed(mousePos);
}

void EndlessGameWidget::dealMoved(QPointF mousePos, Qt::MouseButton button)
{
  // Record the current position
  currentPos = mousePos;

  // Clear user moving elimination hints
  effectPainter->clearUserMovingEliminationHints();

  // Let the gesture controller to deal the move event
  gestureController->dealMoved(mousePos);
}

void EndlessGameWidget::dealReleased(QPointF mousePos, Qt::MouseButton button)
{
  if (itemAtPressPos != NULL)
  {
    if (itemAtPressPos == flame && flame->notEmpty())
    {
      int index = gameboardInfo->indexOfMousePosition(mousePos);
      if (index != -1)
      {
        // Tell the controller to eliminate the balls
        controller->flameAt(index);

        // Add effects to effect painter
        effectPainter->explodeAt(index);
        effectPainter->flash();

        // Minus the value of the flame
        flame->minusOne();
      }
    }
    else if (itemAtPressPos == star && star->notEmpty())
    {
      int index = gameboardInfo->indexOfMousePosition(mousePos);
      if (index != -1)
      {
        // Tell the controller to eliminate the balls
        controller->starAt(index);

        // Add effects to effect painter
        effectPainter->lightningAt(index);
        effectPainter->flash();

        // Minus the value of the flame
        star->minusOne();
      }
    }
    else if (itemAtPressPos == hint &&
             distanceOfTwoPoints(mousePos,
                                 toScene(hint->getPos().x(),
                                         hint->getPos().y())) < 30)
    {
      // Reduce the score
      int score = qMax(progressBar->getCurrent() - 10,
                       progressBar->getMin());
      progressBar->setCurrent(score);

      // Show the hint
      showHint();
    }
    else if (itemAtPressPos == resetItem &&
             distanceOfTwoPoints(mousePos,
                                 toScene(resetItem->getPos().x(),
                                         resetItem->getPos().y())) < 30)
    {
      // Create the reset widget
      ResetWidget *w = new ResetWidget();

      // Connect
      connect(w, SIGNAL(confirm()), this, SLOT(reset()));

      // Give control to it
      emit giveControlTo(w, false);
    }
    else if (itemAtPressPos == exitItem &&
             distanceOfTwoPoints(mousePos,
                                 toScene(exitItem->getPos().x(),
                                         exitItem->getPos().y())) < 30)
    {
      // Quit game
      quitGame();
      return;
    }
  }

  // Clear user moving elimination hints
  effectPainter->clearUserMovingEliminationHints();
  itemAtPressPos = NULL;

  // Let the gesture controller to deal the release event
  gestureController->dealReleased(mousePos);
}

void EndlessGameWidget::getForcus()
{
  // Start the timer
  t->start();
}

void EndlessGameWidget::advance()
{
  // Go to next stage if the score has been reached
  if (progressBar->getCurrent() >= progressBar->getMax())
  {
    nextStage();
    return;
  }

  // Add the frame count
  ++frameCount;

  // Advance the controller
  controller->advance();
}

void EndlessGameWidget::eliminated(int count)
{
  // Set the score
  progressBar->setCurrent(progressBar->getCurrent() + count);

  // Reset the highest score if neccessary
  if (progressBar->getCurrent() > hightestScore->getValue())
  {
    OtherGameInit::testHighest(getIndex(), progressBar->getCurrent());
    hightestScore->setValue(progressBar->getCurrent());
  }
}

void EndlessGameWidget::dealStableEliminate(Connections connections)
{
  // Calculate the bonus
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
    }
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
}

void EndlessGameWidget::dealUserMovingEliminate(Connections connections)
{
  // Add moving elimination hint if neccessary
  if (rule->gestureAllowed(AbstractRule::Rotate))
  {
    effectPainter->clearUserMovingEliminationHints();
    for (int i = 0;i < connections.connections.size();++i)
    {
      for (int j = 0;j < connections.connections[i]->size();++j)
        effectPainter->userMovingEliminationHintAt(connections.connections[i]->at(j));
    }
  }
}

void EndlessGameWidget::nextStage()
{
  // Record the initial state of next stage
  OtherGameRecord record;
  record.currentLevel = currentLevel->getValue() + 1;
  record.minScore = progressBar->getMax();
  record.currentScore = progressBar->getCurrent();
  record.maxScore = progressBar->getMax() * 2;
  record.flame = flame->getCurrent();
  record.star = star->getCurrent();
  record.balls = NULL;
  OtherGameInit::saveOtherGame(&record,
                               getIndex(),
                               gameboardInfo->totalBallCounts());

  // Create another widget of endless game and give control
  // to it
  EndlessGameWidget *nextStage;
  if (rule->gestureAllowed(AbstractRule::Swap))
    nextStage = new EndlessGameWidget(AbstractRule::Swap);
  else
    nextStage = new EndlessGameWidget(AbstractRule::Rotate);
  emit giveControlTo(nextStage, true);
  delete this;
}

int EndlessGameWidget::getIndex()
{
  return 2 + (rule->gestureAllowed(AbstractRule::Rotate) ? 1 : 0);
}

void EndlessGameWidget::reset()
{
  OtherGameInit::clearGame(getIndex());
  EndlessGameWidget *resetGame;
  t->stop();
  if (rule->gestureAllowed(AbstractRule::Swap))
    resetGame = new EndlessGameWidget(AbstractRule::Swap);
  else
    resetGame = new EndlessGameWidget(AbstractRule::Rotate);
  emit giveControlTo(resetGame, true);
  delete this;
}
