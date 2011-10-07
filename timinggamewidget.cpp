#include "timinggamewidget.h"

#include <QPainter>
#include <QPixmap>
#include <QPointF>
#include <QTimer>
#include <QFont>
#include <QPainterPath>
#include "effectpainter.h"
#include "rules.h"
//#include "sixtyonegameboardinfo.h"
#include "connections.h"
#include "corecontroller.h"
#include "gesturecontroller.h"
#include "basicpainter.h"
#include "gamecommonitems.h"
#include "thirtysevengameboardinfo.h"
#include "othergameinit.h"
#include "resetwidget.h"
#include "pausewidget.h"
#include "gameoverwidget.h"
#include "publicgamesounds.h"
#include "ball.h"
#include "statistic.h"

extern Statistic statistic;

#define FONT_FAMILY           ""
#define FONT_DIGIT_SIZE       60

#define START_ANIM_LAST_TIME  60
#define START_ANIM_SEP_TIME   (START_ANIM_LAST_TIME * 2/ 3)

#define END_ANIM_INTERVAL     4
#define END_ANIM_LAST_TIME    9

#define FLAME_SCORE           7
#define STAR_SCORE            19

/**
 * @brief Get the font of a description.
 */
QFont font(double size)
{
  QFont f;
  f.setFamily(FONT_FAMILY);
  f.setBold(true);
  f.setPointSize(size);
  return f;
}

TimingGameWidget::TimingGameWidget(AbstractRule::Gesture gesture) :
    frameCount(0),
    startAnimCount(0),
    timeUp(false)
{
  // Create the rule
  if (gesture == AbstractRule::Swap)
    rule = new SwapTimingGameRule();
  else
    rule = new RotateTimingGameRule();

  // Create the gameboard info
  gameboardInfo = new ThirtySevenGameBoardInfo();

  // Create the controller
  controller = new CoreController(rule, gameboardInfo, NULL);

  // Move the balls to the correct position
  // and avoid elimination at the beginning
  controller->fillAllBlanks();
  for (int i = 0;i < 100;++i)
    controller->advance();

  for (int i = 0;i < gameboardInfo->totalBallCounts();++i)
    if (controller->balls[i])
      controller->balls[i]->setState(Ball::JustCreated);

  controller->autoRotate();

  //  Create the effect painter
  effectPainter = new EffectPainter(gameboardInfo);

  // Create the gesture controller
  gestureController = new GestureController(rule,
                                            gameboardInfo,
                                            controller,
                                            effectPainter);

  // Create the items and initialize them
  hightestScore = new IntegerItem();
  hightestScore->setPos(QPointF(0.1, 0.1));
  hightestScore->setValue(OtherGameInit::getHighest(getIndex()));
  hightestScore->setHint("Highest Score");
  myItems.push_back(hightestScore);

  currentScore = new IntegerItem();
  currentScore->setPos(QPointF(0.1, 0.25));
  currentScore->setValue(0);
  currentScore->setHint("Current Score");
  myItems.push_back(currentScore);

  timeBar = new VerticalProgressBarItem();
  timeBar->setPos(QPointF(0.25, 0.5));
  timeBar->setCurrent(60);
  timeBar->setMin(0);
  timeBar->setMax(60);
  myItems.push_back(timeBar);

  flame = new FlameItem();
  flame->setPos(QPointF(0.065, 0.45));
  flame->setCurrent(0);
  myItems.push_back(flame);

  star = new StarItem();
  star->setPos(QPointF(0.135, 0.45));
  star->setCurrent(0);
  myItems.push_back(star);

  hint = new ButtonItem("Hint");
  hint->setPos(QPointF(0.1, 0.6));
  myItems.push_back(hint);

  resetItem = new ButtonItem("Reset");
  resetItem->setPos(QPointF(0.1, 0.8));
  myItems.push_back(resetItem);

  pauseItem = new ButtonItem("Pause");
  pauseItem->setPos(QPointF(0.1, 0.7));
  myItems.push_back(pauseItem);

  exitItem = new ButtonItem("Exit");
  exitItem->setPos(QPointF(0.1, 0.9));
  myItems.push_back(exitItem);

  // No items was chosen
  itemAtPressPos = NULL;

  // Connect signals and slots
  connect(controller,
          SIGNAL(goodMove()),
          this,
          SLOT(goodMove()));
  connect(controller,
          SIGNAL(badMove()),
          this,
          SLOT(badMove()));
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

  // Create the timers and connect signals and slots
  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));

  oneSecondTimer = new QTimer();
  oneSecondTimer->setInterval(1000);
  connect(oneSecondTimer, SIGNAL(timeout()), this, SLOT(oneSecond()));
}

void TimingGameWidget::makePixmap(
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

TimingGameWidget::~TimingGameWidget()
{
  // Stop the timer
  t->stop();
  oneSecondTimer->stop();
  // Release the space
  delete t;
  delete oneSecondTimer;
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
  delete controller;
  delete gameboardInfo;
  delete rule;
  delete gestureController;
  delete effectPainter;
}

void TimingGameWidget::makeBasicPixmap(
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

  // Get the balls
  Ball **balls = controller->balls;

  // Paint the background
  BasicPainter::paintBackGround(BasicPainter::Game37,
                                painter,
                                width,
                                height,
                                frameCount);

  // Paint the basic balls
  BasicPainter::paintBasicBalls
      (gameboardInfo,
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

#ifdef USE_PIXMAP
  // End the paint and release the space
  painter->end();
  delete painter;
#endif
}

void TimingGameWidget::addEffect(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                  int width,
                                  int height)
{
#ifdef USE_PIXMAP
  // Get the painter
  QPainter *painter = new QPainter(&pixmap);
#endif

  // Calculte the bonus hint and show it
  QPointF pos = currentPos;
  pos.setX(currentPos.x() *
           width /
           gameboardInfo->width());
  pos.setY(currentPos.y() *
           height /
           gameboardInfo->height());
  effectPainter->clearBonusEliminationHints();
  if (itemAtPressPos != NULL)
  {
    if (itemAtPressPos == flame && flame->notEmpty())
    {
      flame->paintLocatingIcon(painter, width, height, pos, frameCount);
      int index =
          gameboardInfo->indexOfPosition(currentPos);
      flame->paintInfluencedArea(index,
                                 gameboardInfo,
                                 effectPainter,
                                 frameCount);
    }
    else if (itemAtPressPos == star && star->notEmpty())
    {
      star->paintLocatingIcon(painter, width, height, pos, frameCount);
      int index =
          gameboardInfo->indexOfPosition(currentPos);
      star->paintInfluencedArea(index,
                                gameboardInfo,
                                effectPainter,
                                frameCount);
    }
  }

  // Paint the effects
  effectPainter->paint(painter,
                       width *
                       1.0 /
                       gameboardInfo->width(),
                       height *
                       1.0 /
                       gameboardInfo->height());

  if (startAnimCount <= START_ANIM_LAST_TIME)
  {
    QPointF center = gameboardInfo->centerPos();
    double size = qMax(1,
                       qMin(FONT_DIGIT_SIZE *
                            startAnimCount /
                            START_ANIM_SEP_TIME,
                            FONT_DIGIT_SIZE));
    double opacity = startAnimCount < START_ANIM_SEP_TIME ?
                     1.0 * startAnimCount / START_ANIM_SEP_TIME :
                     1 - 1.0 * (startAnimCount - START_ANIM_SEP_TIME) /
                     (START_ANIM_LAST_TIME - START_ANIM_SEP_TIME);
    painter->setOpacity(opacity);
    QPainterPath path;
    path.addText(- size * 2 / 3, 0, font(size), "GO");
    painter->setWindow(0, 0, gameboardInfo->width(), gameboardInfo->height());
    painter->translate(center);
    QPen pen = QPen(QColor(  0, 255, 255));
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawPath(path);
    painter->fillPath(path, QBrush(QColor(0, 0, 0)));
    painter->setOpacity(1);
    painter->translate(-center.x(), -center.y());
    painter->setWindow(0, 0, width, height);
  }

  if (timeUp)
  {
    for (QList<int>::Iterator itr1 = endAnimCount.begin(),
                              itr2 = endAnimBonusKind.begin();
         itr1 != endAnimCount.end();
         ++itr1, ++itr2)
    {
      painter->setOpacity(1.0 * (END_ANIM_LAST_TIME - *itr1) / END_ANIM_LAST_TIME);
      AbstractBonusItem *bonusItem = ((*itr2) == 0) ? flame : star;
      pos.setX((currentScore->getPos().x() * (*itr1) +
                bonusItem->getPos().x() * (END_ANIM_LAST_TIME - *itr1)) /
               END_ANIM_LAST_TIME *
               width);
      pos.setY((currentScore->getPos().y() * (*itr1) +
                bonusItem->getPos().y() * (END_ANIM_LAST_TIME - *itr1)) /
               END_ANIM_LAST_TIME *
               height);
      bonusItem->paintLocatingIcon(painter, width, height, pos, frameCount);
    }
  }

  ++startAnimCount;

#ifdef USE_PIXMAP
  // End the paint and release the space
  painter->end();
  delete painter;
#endif

  // Advance the effect painter
  effectPainter->advance();
}

QPointF TimingGameWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * gameboardInfo->width(),
                 yRate * gameboardInfo->height());
}

void TimingGameWidget::showHint()
{
  // Get the index of the hint
  int hintOnBoard = controller->hint();

  // Show the hint
  effectPainter->hintAt(gameboardInfo->positionOfIndex(hintOnBoard),
                        rule->gestureAllowed(AbstractRule::Rotate));
}

void TimingGameWidget::gameOver()
{
  // Add sound effect
  PublicGameSounds::addSound(PublicGameSounds::GameOver);

  // Test the highest score
  OtherGameInit::testHighest(getIndex(), currentScore->getValue());

  // Create the game over widget and give control to it
  GameOverWidget *w = new GameOverWidget(getIndex(),
                                         currentScore->getValue());
  emit giveControlTo(w, true);
  delete this;
}

void TimingGameWidget::quitGame()
{
  emit giveControlTo(NULL, true);
  delete this;
}

void TimingGameWidget::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  if (timeUp)
    return;

  // Choose the correct item at press position
  currentPos = mousePos;
  if (flame->in(mousePos, gameboardInfo->width(), gameboardInfo->height()))
    itemAtPressPos = flame;
  else if (star->in(mousePos, gameboardInfo->width(), gameboardInfo->height()))
    itemAtPressPos = star;
  else if (hint->in(mousePos, gameboardInfo->width(), gameboardInfo->height()))
    itemAtPressPos = hint;
  else if (resetItem->in(mousePos,
                         gameboardInfo->width(),
                         gameboardInfo->height()))
    itemAtPressPos = resetItem;
  else if (pauseItem->in(mousePos,
                         gameboardInfo->width(),
                         gameboardInfo->height()))
    itemAtPressPos = pauseItem;
  else if (exitItem->in(mousePos,
                        gameboardInfo->width(),
                        gameboardInfo->height()))
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

void TimingGameWidget::dealMoved(QPointF mousePos, Qt::MouseButton button)
{
  if (timeUp)
    return;

  // Record the current position
  currentPos = mousePos;

  // Clear user moving elimination hints
  effectPainter->clearUserMovingEliminationHints();

  // Let the gesture controller to deal the move event
  gestureController->dealMoved(mousePos);
}

void TimingGameWidget::dealReleased(QPointF mousePos, Qt::MouseButton button)
{
  if (timeUp)
    return;

  if (itemAtPressPos != NULL)
  {
    if (itemAtPressPos == flame && flame->notEmpty())
    {
      int index = gameboardInfo->indexOfPosition(mousePos);
      if (index != -1)
      {
        // Add sound effect
        PublicGameSounds::addSound(PublicGameSounds::UseFlame);

        // Tell the controller to eliminate the balls
        controller->flameAt(index);

        // Add effects to effect painter
        effectPainter->explodeAt(index);
        effectPainter->flash();

        // Minus the value of the flame
        flame->minusOne();

        statistic.changeStatistic(Statistic::FlameUsedCount, 1, true);
      }
    }
    else if (itemAtPressPos == star && star->notEmpty())
    {
      int index = gameboardInfo->indexOfPosition(mousePos);
      if (index != -1)
      {
        // Add sound effect
        PublicGameSounds::addSound(PublicGameSounds::UseStar);

        // Tell the controller to eliminate the balls
        controller->starAt(index);

        // Add effects to effect painter
        effectPainter->lightningAt(index);
        effectPainter->flash();

        // Minus the value of the flame
        star->minusOne();

        statistic.changeStatistic(Statistic::StarUsedCount, 1, true);
      }
    }
    else if (itemAtPressPos == hint &&
             hint->in(mousePos,
                      gameboardInfo->width(),
                      gameboardInfo->height()))
    {
      // Reduce the score
      int score = qMax(currentScore->getValue() - 10, 0);
      currentScore->setValue(score);

      // Show the hint
      showHint();
    }
    else if (itemAtPressPos == pauseItem &&
             pauseItem->in(mousePos,
                           gameboardInfo->width(),
                           gameboardInfo->height()))
    {
      // Pause
      t->stop();
      oneSecondTimer->stop();

      // Show pause widget
      PauseWidget *w = new PauseWidget();
      connect(w, SIGNAL(resume()), this, SLOT(resume()));
      emit giveControlTo(w, false);
      return;
    }
    else if (itemAtPressPos == resetItem &&
             resetItem->in(mousePos,
                           gameboardInfo->width(),
                           gameboardInfo->height()))
    {
      // Pause
      t->stop();
      oneSecondTimer->stop();

      // Show reset widget
      ResetWidget *w = new ResetWidget();
      connect(w, SIGNAL(confirm()), this, SLOT(reset()));
      emit giveControlTo(w, false);
    }
    else if (itemAtPressPos == exitItem &&
             exitItem->in(mousePos,
                          gameboardInfo->width(),
                          gameboardInfo->height()))
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

void TimingGameWidget::getForcus()
{
  // Start the timers
  t->start();
  oneSecondTimer->start();
}

void TimingGameWidget::advance()
{
  // Add the frame count
  ++frameCount;

  // Advance the controller
  controller->advance();

  if (timeUp)
  {
    for (QList<int>::Iterator itr = endAnimCount.begin();
         itr != endAnimCount.end();
         ++itr)
      ++(*itr);

    if (!endAnimCount.isEmpty())
      if (endAnimCount[0] >= END_ANIM_LAST_TIME)
      {
        currentScore->setValue(currentScore->getValue() +
                               ((endAnimBonusKind[0] == 0) ?
                                FLAME_SCORE :
                                STAR_SCORE));
        endAnimCount.pop_front();
        endAnimBonusKind.pop_front();
      }
    if (endAnimCount.isEmpty())
    {
      if (flame->getCurrent() > 0)
      {
        flame->minusOne();
        endAnimCount.push_back(0);
        endAnimBonusKind.push_back(0);
      }
      else if (star->getCurrent() > 0)
      {
        star->minusOne();
        endAnimCount.push_back(0);
        endAnimBonusKind.push_back(1);
      }
      else
      {

        bool allStable = true;
        for (int i = 0;i < gameboardInfo->totalBallCounts();++i)
          if ((!controller->balls[i]) ||
              controller->balls[i]->getState() != Ball::Stable)
          {
            allStable = false;
            break;
          }

        if (allStable)
        {
          gameOver();
          return;
        }
      }
    }
    else
    {
      if (endAnimCount.last() == END_ANIM_INTERVAL)
      {
        if (flame->getCurrent() > 0)
        {
          flame->minusOne();
          endAnimCount.push_back(0);
          endAnimBonusKind.push_back(0);
        }
        else if (star->getCurrent() > 0)
        {
          star->minusOne();
          endAnimCount.push_back(0);
          endAnimBonusKind.push_back(1);
        }
      }
    }
  }
}

void TimingGameWidget::eliminated(int count)
{
  // Add sound effect if neccessary
  if (count > 0)
    PublicGameSounds::addEliminate(count);

  // Set the score
  currentScore->setValue(currentScore->getValue() + count);

//  // Reset the highest score if neccessary
//  if (currentScore->getValue() > hightestScore->getValue())
//  {
//    OtherGameInit::testHighest(getIndex(), currentScore->getValue());
//    hightestScore->setValue(currentScore->getValue());
//  }
}

void TimingGameWidget::dealStableEliminate(Connections connections)
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
      {
        // Add sound effect
        PublicGameSounds::addSound(PublicGameSounds::GetFlame);

        // Get a flame
        flame->addOne();

        statistic.changeStatistic(Statistic::FlameGetCount, 1, true);
      }
      if (connectionCountOfThePosition >= 3)
      {
        // Add sound effect
        PublicGameSounds::addSound(PublicGameSounds::GetStar);

        // Get a star
        star->addOne();

        statistic.changeStatistic(Statistic::StarGetCount, 1, true);
      }
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
    {
      // Add sound effect
      PublicGameSounds::addSound(PublicGameSounds::GetFlame);

      // Get a flame
      flame->addOne();

      statistic.changeStatistic(Statistic::FlameGetCount, 1, true);
    }
    if (size >= 5)
    {
      // Add sound effect
      PublicGameSounds::addSound(PublicGameSounds::GetStar);

      // Get a star
      star->addOne();

      statistic.changeStatistic(Statistic::StarGetCount, 1, true);
    }
  }
}

void TimingGameWidget::dealUserMovingEliminate(Connections connections)
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

int TimingGameWidget::getIndex()
{
  return 4 + (rule->gestureAllowed(AbstractRule::Rotate) ? 1 : 0);
}

void TimingGameWidget::reset()
{
  TimingGameWidget *resetGame;
  if (rule->gestureAllowed(AbstractRule::Swap))
    resetGame = new TimingGameWidget(AbstractRule::Swap);
  else
    resetGame = new TimingGameWidget(AbstractRule::Rotate);
  emit giveControlTo(resetGame, true);
  delete this;
}

void TimingGameWidget::oneSecond()
{
  // Set the time
  timeBar->setCurrent(timeBar->getCurrent() - 1);

  // Game over when time up
  if (timeBar->getCurrent() <= 0)
  {
    timeUp = true;
    oneSecondTimer->stop();
  }
}

void TimingGameWidget::resume()
{
  // Start the timers
  t->start();
  oneSecondTimer->start();
}

void TimingGameWidget::goodMove()
{
  // Add sound effect
  PublicGameSounds::addSound(PublicGameSounds::GoodMove);

  statistic.changeStatistic(Statistic::GoodMoveCount, 1, true);
}

void TimingGameWidget::badMove()
{
  // Add sound effect
  PublicGameSounds::addSound(PublicGameSounds::BadMove);

  statistic.changeStatistic(Statistic::BadMoveCount, 1, true);
}
