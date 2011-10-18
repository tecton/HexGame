#include "twoplayertiminggamewidget2.h"

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

#include <QDebug>

extern Statistic statistic;

#define FONT_DIGIT_SIZE       90

#define PEN_WIDTH             10

#define START_ANIM_LAST_TIME  5
#define START_ANIM_SEP_TIME   (START_ANIM_LAST_TIME * 2/ 3)

#define END_BONUS_ANIM_INTERVAL     4
#define END_BONUS_ANIM_LAST_TIME    9

#define END_ANIM_LAST_TIME    20

#define FLAME_SCORE           7
#define STAR_SCORE            19

#define GAME1_X_FROM     50
#define GAME1_X_TO       400
#define GAME2_X_FROM     624
#define GAME2_X_TO       974

#define GAME1_WIDTH      LOGICAL_HEIGHT
#define GAME1_HEIGHT     (GAME1_X_TO - GAME1_X_FROM)
#define GAME2_WIDTH      LOGICAL_HEIGHT
#define GAME2_HEIGHT     (GAME2_X_TO - GAME2_X_FROM)

#define LOGICAL_WIDTH    1024
#define LOGICAL_HEIGHT   600

#define LINENEAR_COLOR_0 QColor( 50, 255, 255, 200)
#define LINENEAR_COLOR_1 QColor(250, 255, 255, 200)

/**
 * @brief Get the font of a description.
 */
QFont goFont(double size)
{
  QFont f;
  f.setBold(true);
  f.setPointSize(size);
  return f;
}

TwoPlayerTimingGameWidget2::TwoPlayerTimingGameWidget2(AbstractRule::Gesture gesture) :
    frameCount(0),
    startAnimCount(0),
    endAnimCount(-1),
    timeUp(false)
{
//  QFont f;
//  f.setPixelSize(FONT_DIGIT_SIZE);

//  youWin.addText(- FONT_DIGIT_SIZE * 9 / 4, 0, f, "You Win~~");
//  youLose.addText(- FONT_DIGIT_SIZE * 11 / 4, 0, f, "You Lose-.-");
//  drawGame.addText(- FONT_DIGIT_SIZE * 9 / 4, 0, f, "Draw Game");

  // Create the rule
  if (gesture == AbstractRule::Swap)
    rule = new SwapTimingGameRule();
  else
    rule = new RotateTimingGameRule();

  // Create the gameboard info
  gameboardInfo = new ThirtySevenGameBoardInfo();

  // Create the controller
  controller1 = new CoreController(rule, gameboardInfo, NULL);

  // Create the controller
  controller2 = new CoreController(rule, gameboardInfo, NULL);

  // Move the balls to the correct position
  // and avoid elimination at the beginning
  controller1->fillAllBlanks();
  do
  {
    controller1->setNeedTestStableEliminate(true);
    for (int i = 0;i < gameboardInfo->totalBallCounts();++i)
      if (controller1->balls[i])
        controller1->balls[i]->moveToStablePos();
  } while (controller1->advance());

  for (int i = 0;i < gameboardInfo->totalBallCounts();++i)
    if (controller1->balls[i])
      controller1->balls[i]->setState(Ball::JustCreated);

  controller1->autoRotate();

  // Move the balls to the correct position
  // and avoid elimination at the beginning
  controller2->fillAllBlanks();
  do
  {
    controller2->setNeedTestStableEliminate(true);
    for (int i = 0;i < gameboardInfo->totalBallCounts();++i)
      if (controller2->balls[i])
        controller2->balls[i]->moveToStablePos();
  } while (controller2->advance());

  for (int i = 0;i < gameboardInfo->totalBallCounts();++i)
    if (controller2->balls[i])
      controller2->balls[i]->setState(Ball::JustCreated);

  controller1->autoRotate();

  // Create the gesture controller
  gestureController1 = new GestureController(rule,
                                             gameboardInfo,
                                             controller1,
                                             NULL);

  // Create the gesture controller
  gestureController2 = new GestureController(rule,
                                             gameboardInfo,
                                             controller2,
                                             NULL);

  currentScore1 = new IntegerItem();
  currentScore1->setPos(QPointF(0.275, 0.15));
  currentScore1->setValue(0);
  currentScore1->setHint("Current Score");
  myItems.push_back(currentScore1);

  currentScore2 = new IntegerItem();
  currentScore2->setPos(QPointF(0.725, 0.85));
  currentScore2->setValue(0);
  currentScore2->setHint("Current Score");
  myItems.push_back(currentScore2);

  timeBar = new VerticalProgressBarItem();
  timeBar->setPos(QPointF(0.5, 0.45));
  timeBar->setCurrent(60);
  timeBar->setMin(0);
  timeBar->setMax(60);
  myItems.push_back(timeBar);

  flame1 = new FlameItem();
  flame1->setPos(QPointF(0.175, 0.15));
  flame1->setCurrent(0);
  flame1->setRotation(90);
  myItems.push_back(flame1);

  flame2 = new FlameItem();
  flame2->setPos(QPointF(0.825, 0.85));
  flame2->setCurrent(0);
  flame2->setRotation(-90);
  myItems.push_back(flame2);

  star1 = new StarItem();
  star1->setPos(QPointF(0.1, 0.15));
  star1->setCurrent(0);
  star1->setRotation(90);
  myItems.push_back(star1);

  star2 = new StarItem();
  star2->setPos(QPointF(0.9, 0.85));
  star2->setCurrent(0);
  star2->setRotation(-90);
  myItems.push_back(star2);

  resetItem = new ButtonItem("Reset");
  resetItem->setPos(QPointF(0.5, 0.875));
  myItems.push_back(resetItem);

  pauseItem = new ButtonItem("Pause");
  pauseItem->setPos(QPointF(0.5, 0.8));
  myItems.push_back(pauseItem);

  exitItem = new ButtonItem("Exit");
  exitItem->setPos(QPointF(0.5, 0.95));
  myItems.push_back(exitItem);

  // No items was chosen
  itemAtPressPos1 = NULL;

  // No items was chosen
  itemAtPressPos2 = NULL;

  // No items was chosen
  itemAtPressPos = NULL;

  // Connect signals and slots
  connect(controller1,
          SIGNAL(goodMove()),
          this,
          SLOT(goodMove()));
  connect(controller1,
          SIGNAL(badMove()),
          this,
          SLOT(badMove()));
  connect(controller1,
          SIGNAL(stableEliminateTested(Connections)),
          this,
          SLOT(dealStableEliminate1(Connections)));
  connect(controller1,
          SIGNAL(eliminated(int)),
          this,
          SLOT(eliminated1(int)));

  connect(controller2,
          SIGNAL(goodMove()),
          this,
          SLOT(goodMove()));
  connect(controller2,
          SIGNAL(badMove()),
          this,
          SLOT(badMove()));
  connect(controller2,
          SIGNAL(stableEliminateTested(Connections)),
          this,
          SLOT(dealStableEliminate2(Connections)));
  connect(controller2,
          SIGNAL(eliminated(int)),
          this,
          SLOT(eliminated2(int)));

  // Create the timers and connect signals and slots
  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));

  oneSecondTimer = new QTimer();
  oneSecondTimer->setInterval(1000);
  connect(oneSecondTimer, SIGNAL(timeout()), this, SLOT(oneSecond()));
}

void TwoPlayerTimingGameWidget2::makePixmap(
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

TwoPlayerTimingGameWidget2::~TwoPlayerTimingGameWidget2()
{
  // Stop the timer
  t->stop();
  oneSecondTimer->stop();
  // Release the space
  delete t;
  delete oneSecondTimer;
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
  delete controller1;
  delete controller2;
  delete gameboardInfo;
  delete rule;
  delete gestureController1;
  delete gestureController2;
}

void TwoPlayerTimingGameWidget2::makeBasicPixmap(
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
  Ball **balls = controller1->balls;

  QPointF *positions = new QPointF[gameboardInfo->totalBallCounts()];
  for (int i = 0;i < gameboardInfo->totalBallCounts();++i)
    if (balls[i])
      positions[i] = game1ToGlobal(balls[i]->pos());

  // Paint the basic balls
  BasicPainter::paintBasicBalls
      (balls,
       gameboardInfo->totalBallCounts(),
       painter,
       1,
       1,
       frameCount,
       positions,
       true);

  balls = controller2->balls;
  for (int i = 0;i < gameboardInfo->totalBallCounts();++i)
    if (balls[i])
      positions[i] = game2ToGlobal(balls[i]->pos());

  // Paint the basic balls
  BasicPainter::paintBasicBalls
      (balls,
       gameboardInfo->totalBallCounts(),
       painter,
       1,
       1,
       frameCount,
       positions,
       true);

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

void TwoPlayerTimingGameWidget2::addEffect(
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

//  if (startAnimCount <= START_ANIM_LAST_TIME)
//  {
//    QPointF center = gameboardInfo->centerPos();
//    double size = qMax(1,
//                       qMin(FONT_DIGIT_SIZE *
//                            startAnimCount /
//                            START_ANIM_SEP_TIME,
//                            FONT_DIGIT_SIZE));
//    double opacity = startAnimCount < START_ANIM_SEP_TIME ?
//                     1.0 * startAnimCount / START_ANIM_SEP_TIME :
//                     1 - 1.0 * (startAnimCount - START_ANIM_SEP_TIME) /
//                     (START_ANIM_LAST_TIME - START_ANIM_SEP_TIME);
//    painter->setOpacity(opacity);
//    QPainterPath path;
//    path.addText(- size * 2 / 3, 0, goFont(size), "GO");

//    painter->scale(1.0 * gameboardInfo->width() / width,
//                   1.0 * gameboardInfo->height() / height);

//    QPen pen = QPen(QColor( 50, 255, 255, 100));
//    pen.setWidth(PEN_WIDTH * startAnimCount / START_ANIM_SEP_TIME);
//    painter->setPen(pen);

//    QPointF pos1 = game1ToGlobal(center);
//    painter->translate(pos1.x(), pos1.y());
//    painter->rotate(90);

//    painter->drawPath(path);
//    painter->fillPath(path, QBrush(QColor(0, 0, 0)));

//    painter->rotate(-90);
//    painter->translate(-pos1.x(), -pos1.y());

//    pos1 = game2ToGlobal(center);
//    painter->translate(pos1.x(), pos1.y());
//    painter->rotate(-90);

//    painter->drawPath(path);
//    painter->fillPath(path, QBrush(QColor(0, 0, 0)));

//    painter->rotate(90);
//    painter->translate(-pos1.x(), -pos1.y());

//    painter->setOpacity(1);

//    painter->scale(1.0 * width / gameboardInfo->width(),
//                   1.0 * height / gameboardInfo->height());
//  }

  if (timeUp)
  {
    QPointF pos;
    for (QList<int>::Iterator itr1 = endAnimCount1.begin(),
                              itr2 = endAnimBonusKind1.begin();
         itr1 != endAnimCount1.end();
         ++itr1, ++itr2)
    {
      painter->setOpacity(1.0 * (END_BONUS_ANIM_LAST_TIME - *itr1) / END_BONUS_ANIM_LAST_TIME);
      AbstractBonusItem *bonusItem = ((*itr2) == 0) ? flame1 : star1;
      pos.setX((currentScore1->getPos().x() * (*itr1) +
                bonusItem->getPos().x() * (END_BONUS_ANIM_LAST_TIME - *itr1)) /
               END_BONUS_ANIM_LAST_TIME *
               width);
      pos.setY((currentScore1->getPos().y() * (*itr1) +
                bonusItem->getPos().y() * (END_BONUS_ANIM_LAST_TIME - *itr1)) /
               END_BONUS_ANIM_LAST_TIME *
               height);
      bonusItem->paintLocatingIcon(painter, width, height, pos, frameCount);
      painter->setOpacity(1);
    }
    for (QList<int>::Iterator itr1 = endAnimCount2.begin(),
                              itr2 = endAnimBonusKind2.begin();
         itr1 != endAnimCount2.end();
         ++itr1, ++itr2)
    {
      painter->setOpacity(1.0 * (END_BONUS_ANIM_LAST_TIME - *itr1) / END_BONUS_ANIM_LAST_TIME);
      AbstractBonusItem *bonusItem = ((*itr2) == 0) ? flame2 : star2;
      pos.setX((currentScore2->getPos().x() * (*itr1) +
                bonusItem->getPos().x() * (END_BONUS_ANIM_LAST_TIME - *itr1)) /
               END_BONUS_ANIM_LAST_TIME *
               width);
      pos.setY((currentScore2->getPos().y() * (*itr1) +
                bonusItem->getPos().y() * (END_BONUS_ANIM_LAST_TIME - *itr1)) /
               END_BONUS_ANIM_LAST_TIME *
               height);
      bonusItem->paintLocatingIcon(painter, width, height, pos, frameCount);
      painter->setOpacity(1);
    }
  }

  double xRate = 1.0 * width / LOGICAL_WIDTH;
  double yRate = 1.0 * height / LOGICAL_HEIGHT;

  if (endAnimCount >= 0)
  {
    BasicPainter::darken(painter, width, height);
    painter->scale(xRate, yRate);
    painter->setOpacity(qMin(1.0, 1.0 * endAnimCount / END_ANIM_LAST_TIME));
    QPointF gFrom = QPointF(frameCount * 8, 0);
    QPointF gTo = QPointF(frameCount * 8 - 100, -100);
    QLinearGradient gradient = QLinearGradient(gFrom, gTo);
    gradient.setColorAt(0, LINENEAR_COLOR_0);
    gradient.setColorAt(1, LINENEAR_COLOR_1);
    gradient.setSpread(QGradient::ReflectSpread);
    QBrush brush = QBrush(gradient);
    painter->setPen(QPen(brush, 4));

    painter->translate((GAME1_X_FROM + GAME1_X_TO) / 2,
                       LOGICAL_HEIGHT / 2);
    painter->rotate(90);
    if (currentScore1->getValue() > currentScore2->getValue())
    {
      painter->drawPath(youWin);
      painter->fillPath(youWin, brush);
    }
    else if (currentScore1->getValue() < currentScore2->getValue())
    {
      painter->drawPath(youLose);
      painter->fillPath(youLose, brush);
    }
    else
    {
      painter->drawPath(drawGame);
      painter->fillPath(drawGame, brush);
    }
    painter->rotate(-90);
    painter->translate(-(GAME1_X_FROM + GAME1_X_TO) / 2,
                       -LOGICAL_HEIGHT / 2);

    painter->translate((GAME2_X_FROM + GAME2_X_TO) / 2,
                       LOGICAL_HEIGHT / 2);
    painter->rotate(-90);
    if (currentScore1->getValue() < currentScore2->getValue())
    {
      painter->drawPath(youWin);
      painter->fillPath(youWin, brush);
    }
    else if (currentScore1->getValue() > currentScore2->getValue())
    {
      painter->drawPath(youLose);
      painter->fillPath(youLose, brush);
    }
    else
    {
      painter->drawPath(drawGame);
      painter->fillPath(drawGame, brush);
    }
    painter->rotate(90);
    painter->translate(-(GAME2_X_FROM + GAME2_X_TO) / 2,
                       -LOGICAL_HEIGHT / 2);
    painter->scale(1.0 / xRate, 1.0 / yRate);
  }

  if (endAnimCount != -1)
  {
    ++endAnimCount;
  }

  ++startAnimCount;

#ifdef USE_PIXMAP
  // End the paint and release the space
  painter->end();
  delete painter;
#endif
}

QPointF TwoPlayerTimingGameWidget2::toScene(double xRate, double yRate)
{
  return QPointF(xRate * gameboardInfo->width(),
                 yRate * gameboardInfo->height());
}

void TwoPlayerTimingGameWidget2::gameOver()
{
  emit giveControlTo(NULL, true);
  delete this;
}

void TwoPlayerTimingGameWidget2::quitGame()
{
  emit giveControlTo(NULL, true);
  delete this;
}

int whichGame2(QPointF mousePos)
{
  double x = mousePos.x();
  if (x <= GAME1_X_TO)
    return 1;
  if (x >= GAME2_X_FROM)
    return 2;
  return 0;
}

QPointF TwoPlayerTimingGameWidget2::game1ToGlobal(QPointF pos)
{
  return QPointF(GAME1_X_TO - pos.y() * GAME1_HEIGHT / gameboardInfo->height(),
                 pos.x() * GAME1_WIDTH / gameboardInfo->width());
}

QPointF TwoPlayerTimingGameWidget2::game2ToGlobal(QPointF pos)
{
  return QPointF(GAME2_X_FROM + pos.y() * GAME2_HEIGHT / gameboardInfo->height(),
                 LOGICAL_HEIGHT - pos.x() * GAME1_WIDTH / gameboardInfo->width());
}

void TwoPlayerTimingGameWidget2::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  // Quit if it's a right button
  // May be abandoned later
  if (button == Qt::RightButton)
  {
    quitGame();
    return;
  }

  if (timeUp && endAnimCount < END_ANIM_LAST_TIME)
    return;

  if (endAnimCount >= END_ANIM_LAST_TIME)
  {
    quitGame();
    return;
  }

  switch (whichGame2(mousePos))
  {
  case 0:
    if (resetItem->in(mousePos, gameboardInfo->width(), gameboardInfo->height()))
      itemAtPressPos = resetItem;
    else if (pauseItem->in(mousePos, gameboardInfo->width(), gameboardInfo->height()))
      itemAtPressPos = pauseItem;
    else if (exitItem->in(mousePos, gameboardInfo->width(), gameboardInfo->height()))
      itemAtPressPos = exitItem;
    else
      itemAtPressPos = NULL;
    break;
  case 1:
    currentPos1 = mousePos;
    if (flame1->in(mousePos, gameboardInfo->width(), gameboardInfo->height()))
      itemAtPressPos1 = flame1;
    else if (star1->in(mousePos, gameboardInfo->width(), gameboardInfo->height()))
      itemAtPressPos1 = star1;
    else
      itemAtPressPos1 = NULL;
    gestureController1->dealPressed(toScene(mousePos.y() / LOGICAL_HEIGHT,
                                            (GAME1_X_TO - mousePos.x()) / GAME1_HEIGHT));
    break;
  case 2:
    currentPos2 = mousePos;
    if (flame2->in(mousePos, gameboardInfo->width(), gameboardInfo->height()))
      itemAtPressPos2 = flame2;
    else if (star2->in(mousePos, gameboardInfo->width(), gameboardInfo->height()))
      itemAtPressPos2 = star2;
    else
      itemAtPressPos2 = NULL;
    gestureController2->dealPressed(toScene(1 - mousePos.y() / LOGICAL_HEIGHT,
                                            (mousePos.x() - GAME2_X_FROM) / GAME2_HEIGHT));
    break;
  default:
    break;
  }
}

void TwoPlayerTimingGameWidget2::dealMoved(QPointF mousePos, Qt::MouseButton button)
{
  if (timeUp && endAnimCount < END_ANIM_LAST_TIME)
    return;

  if (endAnimCount >= END_ANIM_LAST_TIME)
  {
    quitGame();
    return;
  }

  switch (whichGame2(mousePos))
  {
  case 1:
    gestureController1->dealMoved(toScene(mousePos.y() / LOGICAL_HEIGHT,
                                          (GAME1_X_TO - mousePos.x()) / GAME1_HEIGHT));
    break;
  case 2:
    gestureController2->dealMoved(toScene(1 - mousePos.y() / LOGICAL_HEIGHT,
                                          (mousePos.x() - GAME2_X_FROM) / GAME2_HEIGHT));
    break;
  default:
    break;
  }
}

void TwoPlayerTimingGameWidget2::dealReleased(QPointF mousePos, Qt::MouseButton button)
{
  if (timeUp && endAnimCount < END_ANIM_LAST_TIME)
    return;

  if (endAnimCount >= END_ANIM_LAST_TIME)
    quitGame();

  switch (whichGame2(mousePos))
  {
  case 1:
    if (itemAtPressPos1 != NULL)
    {
      QPointF pos = toScene(mousePos.y() / LOGICAL_HEIGHT,
                            (GAME1_X_TO - mousePos.x()) / GAME1_HEIGHT);
      if (itemAtPressPos1 == flame1 && flame1->notEmpty())
      {
        int index = gameboardInfo->indexOfPosition(pos);
        if (index != -1)
        {
          // Add sound effect
          PublicGameSounds::addSound(PublicGameSounds::UseFlame);

          // Tell the controller to eliminate the balls
          controller1->flameAt(index);

          // Minus the value of the flame
          flame1->minusOne();

          statistic.changeStatistic(Statistic::FlameUsedCount, 1, true);
        }
      }
      else if (itemAtPressPos1 == star1 && star1->notEmpty())
      {
        int index = gameboardInfo->indexOfPosition(pos);
        if (index != -1)
        {
          // Add sound effect
          PublicGameSounds::addSound(PublicGameSounds::UseStar);

          // Tell the controller to eliminate the balls
          controller1->starAt(index);

          // Minus the value of the flame
          star1->minusOne();

          statistic.changeStatistic(Statistic::StarUsedCount, 1, true);
        }
      }

      itemAtPressPos1 = NULL;

      // Let the gesture controller to deal the release event
      gestureController1->dealReleased(pos);
      break;
    }
  case 2:
    if (itemAtPressPos2 != NULL)
    {
      QPointF pos = toScene(1 - mousePos.y() / LOGICAL_HEIGHT,
                            (mousePos.x() - GAME2_X_FROM) / GAME2_HEIGHT);
      if (itemAtPressPos2 == flame2 && flame2->notEmpty())
      {
        int index = gameboardInfo->indexOfPosition(pos);
        if (index != -1)
        {
          // Add sound effect
          PublicGameSounds::addSound(PublicGameSounds::UseFlame);

          // Tell the controller to eliminate the balls
          controller2->flameAt(index);

          // Minus the value of the flame
          flame2->minusOne();

          statistic.changeStatistic(Statistic::FlameUsedCount, 1, true);
        }
      }
      else if (itemAtPressPos2 == star2 && star2->notEmpty())
      {
        int index = gameboardInfo->indexOfPosition(pos);
        if (index != -1)
        {
          // Add sound effect
          PublicGameSounds::addSound(PublicGameSounds::UseStar);

          // Tell the controller to eliminate the balls
          controller2->starAt(index);

          // Minus the value of the flame
          star2->minusOne();

          statistic.changeStatistic(Statistic::StarUsedCount, 1, true);
        }
      }

      itemAtPressPos2 = NULL;

      // Let the gesture controller to deal the release event
      gestureController2->dealReleased(pos);
      break;
    }
  case 0:
    if (itemAtPressPos != NULL)
    {
      if (itemAtPressPos == pauseItem &&
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
      break;
  default:
      break;
  }
}

void TwoPlayerTimingGameWidget2::getForcus()
{
  // Start the timers
  t->start();
  oneSecondTimer->start();
}

void TwoPlayerTimingGameWidget2::loseForcus()
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

void TwoPlayerTimingGameWidget2::advance()
{
  // Add the frame count
  ++frameCount;

  // Advance the controller
  controller1->advance();

  // Advance the controller
  controller2->advance();

  if (timeUp)
  {
    for (QList<int>::Iterator itr = endAnimCount1.begin();
         itr != endAnimCount1.end();
         ++itr)
      ++(*itr);

    for (QList<int>::Iterator itr = endAnimCount2.begin();
         itr != endAnimCount2.end();
         ++itr)
      ++(*itr);

    if (!endAnimCount1.isEmpty())
      if (endAnimCount1[0] >= END_BONUS_ANIM_LAST_TIME)
      {
        currentScore1->setValue(currentScore1->getValue() +
                                ((endAnimBonusKind1[0] == 0) ?
                                 FLAME_SCORE :
                                 STAR_SCORE));
        endAnimCount1.pop_front();
        endAnimBonusKind1.pop_front();
      }

    if (!endAnimCount2.isEmpty())
      if (endAnimCount2[0] >= END_BONUS_ANIM_LAST_TIME)
      {
        currentScore2->setValue(currentScore2->getValue() +
                                ((endAnimBonusKind2[0] == 0) ?
                                 FLAME_SCORE :
                                 STAR_SCORE));
        endAnimCount2.pop_front();
        endAnimBonusKind2.pop_front();
      }

    bool allStable = false;
    if (endAnimCount1.isEmpty())
    {
      if (flame1->getCurrent() > 0)
      {
        flame1->minusOne();
        endAnimCount1.push_back(0);
        endAnimBonusKind1.push_back(0);
      }
      else if (star1->getCurrent() > 0)
      {
        star1->minusOne();
        endAnimCount1.push_back(0);
        endAnimBonusKind1.push_back(1);
      }
      else
      {
        allStable = true;
        for (int i = 0;i < gameboardInfo->totalBallCounts();++i)
          if ((!controller2->balls[i]) ||
              controller2->balls[i]->getState() != Ball::Stable)
          {
            allStable = false;
            break;
          }
      }
    }
    else
    {
      if (endAnimCount1.last() == END_BONUS_ANIM_INTERVAL)
      {
        if (flame1->getCurrent() > 0)
        {
          flame1->minusOne();
          endAnimCount1.push_back(0);
          endAnimBonusKind1.push_back(0);
        }
        else if (star1->getCurrent() > 0)
        {
          star1->minusOne();
          endAnimCount1.push_back(0);
          endAnimBonusKind1.push_back(1);
        }
      }
    }

    if (endAnimCount2.isEmpty())
    {
      if (flame2->getCurrent() > 0)
      {
        flame2->minusOne();
        endAnimCount2.push_back(0);
        endAnimBonusKind2.push_back(0);
      }
      else if (star2->getCurrent() > 0)
      {
        star2->minusOne();
        endAnimCount2.push_back(0);
        endAnimBonusKind2.push_back(1);
      }
      else
      {
        for (int i = 0;i < gameboardInfo->totalBallCounts();++i)
          if ((!controller2->balls[i]) ||
              controller2->balls[i]->getState() != Ball::Stable)
          {
            allStable = false;
            break;
          }
        if (allStable && endAnimCount == -1)
        {
          endAnimCount = 0;
          return;
        }
      }
    }
    else
    {
      if (endAnimCount2.last() == END_BONUS_ANIM_INTERVAL)
      {
        if (flame2->getCurrent() > 0)
        {
          flame2->minusOne();
          endAnimCount2.push_back(0);
          endAnimBonusKind2.push_back(0);
        }
        else if (star2->getCurrent() > 0)
        {
          star2->minusOne();
          endAnimCount2.push_back(0);
          endAnimBonusKind2.push_back(1);
        }
      }
    }

  }
}

void TwoPlayerTimingGameWidget2::eliminated1(int count)
{
  // Add sound effect if neccessary
  if (count > 0)
    PublicGameSounds::addEliminate(count);

  // Set the score
  currentScore1->setValue(currentScore1->getValue() + count);
}

void TwoPlayerTimingGameWidget2::eliminated2(int count)
{
  // Add sound effect if neccessary
  if (count > 0)
    PublicGameSounds::addEliminate(count);

  // Set the score
  currentScore2->setValue(currentScore1->getValue() + count);
}

void TwoPlayerTimingGameWidget2::dealStableEliminate1(Connections connections)
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
    if (connectionCountOfThePosition > 1)
    {
      if (connectionCountOfThePosition == 2)
      {
        // Add sound effect
        PublicGameSounds::addSound(PublicGameSounds::GetFlame);

        // Get a flame
        flame1->addOne();

        statistic.changeStatistic(Statistic::FlameGetCount, 1, true);
      }
      if (connectionCountOfThePosition >= 3)
      {
        // Add sound effect
        PublicGameSounds::addSound(PublicGameSounds::GetStar);

        // Get a star
        star1->addOne();

        statistic.changeStatistic(Statistic::StarGetCount, 1, true);
      }
    }
  }
  for (int i = 0;i < connections.connections.size();++i)
  {
    int size = connections.connections[i]->size();
    if (size == 4)
    {
      // Add sound effect
      PublicGameSounds::addSound(PublicGameSounds::GetFlame);

      // Get a flame
      flame1->addOne();

      statistic.changeStatistic(Statistic::FlameGetCount, 1, true);
    }
    if (size >= 5)
    {
      // Add sound effect
      PublicGameSounds::addSound(PublicGameSounds::GetStar);

      // Get a star
      star1->addOne();

      statistic.changeStatistic(Statistic::StarGetCount, 1, true);
    }
  }
}

void TwoPlayerTimingGameWidget2::dealStableEliminate2(Connections connections)
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
    if (connectionCountOfThePosition > 1)
    {
      if (connectionCountOfThePosition == 2)
      {
        // Add sound effect
        PublicGameSounds::addSound(PublicGameSounds::GetFlame);

        // Get a flame
        flame2->addOne();

        statistic.changeStatistic(Statistic::FlameGetCount, 1, true);
      }
      if (connectionCountOfThePosition >= 3)
      {
        // Add sound effect
        PublicGameSounds::addSound(PublicGameSounds::GetStar);

        // Get a star
        star2->addOne();

        statistic.changeStatistic(Statistic::StarGetCount, 1, true);
      }
    }
  }
  for (int i = 0;i < connections.connections.size();++i)
  {
    int size = connections.connections[i]->size();
    if (size == 4)
    {
      // Add sound effect
      PublicGameSounds::addSound(PublicGameSounds::GetFlame);

      // Get a flame
      flame2->addOne();

      statistic.changeStatistic(Statistic::FlameGetCount, 1, true);
    }
    if (size >= 5)
    {
      // Add sound effect
      PublicGameSounds::addSound(PublicGameSounds::GetStar);

      // Get a star
      star2->addOne();

      statistic.changeStatistic(Statistic::StarGetCount, 1, true);
    }
  }
}

void TwoPlayerTimingGameWidget2::reset()
{
  TwoPlayerTimingGameWidget2 *resetGame;
  if (rule->gestureAllowed(AbstractRule::Swap))
    resetGame = new TwoPlayerTimingGameWidget2(AbstractRule::Swap);
  else
    resetGame = new TwoPlayerTimingGameWidget2(AbstractRule::Rotate);
  emit giveControlTo(resetGame, true);
  delete this;
}

void TwoPlayerTimingGameWidget2::oneSecond()
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

void TwoPlayerTimingGameWidget2::resume()
{
  // Start the timers
  t->start();
  oneSecondTimer->start();
}

void TwoPlayerTimingGameWidget2::goodMove()
{
  // Add sound effect
  PublicGameSounds::addSound(PublicGameSounds::GoodMove);

  statistic.changeStatistic(Statistic::GoodMoveCount, 1, true);
}

void TwoPlayerTimingGameWidget2::badMove()
{
  // Add sound effect
  PublicGameSounds::addSound(PublicGameSounds::BadMove);

  statistic.changeStatistic(Statistic::BadMoveCount, 1, true);
}
