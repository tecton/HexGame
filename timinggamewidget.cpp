#include "timinggamewidget.h"

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
#include "pausewidget.h"
#include "gameoverwidget.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

TimingGameWidget::TimingGameWidget(AbstractRule::Gesture gesture) :
    frameCount(0)
{
  if (gesture == AbstractRule::Swap)
    rule = new SwapTimingGameRule();
  else
    rule = new RotateTimingGameRule();
  gameboardInfo = new ThirtySevenGameBoardInfo();

  //TimingGameWidgetSavedInfo savedInfo = readSaved();
  controller = new CoreController(rule, gameboardInfo, NULL);
  controller->fillAllBlanks();
  for (int i = 0;i < 100;++i)
    controller->advance();
  gestureController = new GestureController(rule, gameboardInfo, controller);

  effectPainter = new EffectPainter(gameboardInfo);

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
  flame->setPos(QPointF(0.1, 0.375));
  flame->setCurrent(0);
  myItems.push_back(flame);

  star = new StarItem();
  star->setPos(QPointF(0.1, 0.5));
  star->setCurrent(0);
  myItems.push_back(star);

  hint = new HintItem();
  hint->setPos(QPointF(0.1, 0.6));
  myItems.push_back(hint);

  resetItem = new ResetItem();
  resetItem->setPos(QPointF(0.1, 0.7));
  myItems.push_back(resetItem);

  pauseItem = new PauseItem();
  pauseItem->setPos(QPointF(0.1, 0.8));
  myItems.push_back(pauseItem);

  exitItem = new ExitItem();
  exitItem->setPos(QPointF(0.1, 0.9));
  myItems.push_back(exitItem);

  itemAtPressPos = NULL;

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
          SLOT(elimitated(int)));

  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));

  oneSecondTimer = new QTimer();
  oneSecondTimer->setInterval(1000);
  connect(oneSecondTimer, SIGNAL(timeout()), this, SLOT(oneSecond()));
}

void TimingGameWidget::makePixmap(QPixmap& pixmap, int width, int height)
{
  makeBasicPixmap(pixmap, width, height);
  addEffect(pixmap, width, height);
}

TimingGameWidget::~TimingGameWidget()
{
  t->stop();
  delete t;
  oneSecondTimer->stop();
  delete oneSecondTimer;
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
  delete controller;
  delete gameboardInfo;
  delete rule;
  delete gestureController;
  delete effectPainter;
}

//void TimingGameWidget::init() //
//}

void TimingGameWidget::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
  pixmap = QPixmap(width, height);
  pixmap.fill(Qt::black);
  QPainter *painter = new QPainter(&pixmap);
  Ball **balls = controller->balls;
  BasicPainter::paintBackGround(BasicPainter::Game37,
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
  painter->end();
  delete painter;
}

void TimingGameWidget::addEffect(QPixmap& pixmap, int width, int height)
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

QPointF TimingGameWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * gameboardInfo->width(),
                 yRate * gameboardInfo->height());
}

//TimingGameWidgetSavedInfo TimingGameWidget::readSaved()
//{

//}

void TimingGameWidget::showHint()
{
  int hintOnBoard = controller->hint();
  effectPainter->hintAt(gameboardInfo->centerPositionOfIndex(hintOnBoard),
                        rule->gestureAllowed(AbstractRule::Rotate));
}

void TimingGameWidget::gameOver()
{
  OtherGameInit::testHighest(getIndex(), currentScore->getValue());
  GameOverWidget *w = new GameOverWidget(getIndex(), currentScore->getValue());
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
                               toScene(pauseItem->getPos().x(),
                                       pauseItem->getPos().y())) < 50)
    itemAtPressPos = pauseItem;
  else if (distanceOfTwoPoints(mousePos,
                               toScene(exitItem->getPos().x(),
                                       exitItem->getPos().y())) < 50)
    itemAtPressPos = exitItem;
  else
    itemAtPressPos = NULL;


  if (button == Qt::RightButton)
  {
    quitGame();
    return;
  }
  gestureController->dealPressed(mousePos);
}

void TimingGameWidget::dealMoved(QPointF mousePos, Qt::MouseButton button)
{
  currentPos = mousePos;
  effectPainter->clearUserMovingEliminationHints();
  gestureController->dealMoved(mousePos);
}

void TimingGameWidget::dealReleased(QPointF mousePos, Qt::MouseButton button)
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
                                         hint->getPos().y())) < 30)
    {
      int score = qMax(currentScore->getValue() - 10, 0);
      currentScore->setValue(score);
      showHint();
    }
    else if (itemAtPressPos == pauseItem &&
             distanceOfTwoPoints(mousePos,
                                 toScene(pauseItem->getPos().x(),
                                         pauseItem->getPos().y())) < 30)
    {
      t->stop();
      oneSecondTimer->stop();
      PauseWidget *w = new PauseWidget();
      connect(w, SIGNAL(resume()), this, SLOT(resume()));
      emit giveControlTo(w, false);
      return;
    }
    else if (itemAtPressPos == resetItem &&
             distanceOfTwoPoints(mousePos,
                                 toScene(resetItem->getPos().x(),
                                         resetItem->getPos().y())) < 30)
    {
      t->stop();
      oneSecondTimer->stop();
      ResetWidget *w = new ResetWidget();
      connect(w, SIGNAL(confirm()), this, SLOT(reset()));
      emit giveControlTo(w, false);
    }
    else if (itemAtPressPos == exitItem &&
             distanceOfTwoPoints(mousePos,
                                 toScene(exitItem->getPos().x(),
                                         exitItem->getPos().y())) < 30)
    {
      quitGame();
      return;
    }
  }

  effectPainter->clearUserMovingEliminationHints();
  itemAtPressPos = NULL;
  gestureController->dealReleased(mousePos);
}

void TimingGameWidget::getForcus()
{
  t->start();
  oneSecondTimer->start();
}

void TimingGameWidget::advance()
{
  ++frameCount;
  frameCount = frameCount % 32;
  controller->advance();
//  effectPainter->advance();
}

void TimingGameWidget::elimitated(int count)
{
  currentScore->setValue(currentScore->getValue() + count);
  if (currentScore->getValue() > hightestScore->getValue())
  {
    OtherGameInit::testHighest(getIndex(), currentScore->getValue());
    hightestScore->setValue(currentScore->getValue());
  }
}

void TimingGameWidget::dealStableEliminate(Connections connections)
{
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

void TimingGameWidget::dealUserMovingEliminate(Connections connections)
{
  if (rule->gestureAllowed(AbstractRule::Rotate))
  {
    effectPainter->clearUserMovingEliminationHints();
    for (int i = 0;i < connections.connections.size();++i)
    {
      for (int j = 0;j < connections.connections[i]->size();++j)
        effectPainter->userMovingEliminationHintAt(connections.connections[i]->at(j));
      // TODO:BLABLABLA
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
  timeBar->setCurrent(timeBar->getCurrent() - 1);
  if (timeBar->getCurrent() <= 0)
    gameOver();

}

void TimingGameWidget::resume()
{
  t->start();
  oneSecondTimer->start();
}
