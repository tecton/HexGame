#include "classicgamewidget.h"

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
#include "gameoverwidget.h"

#define LOGICAL_WIDTH  800
#define LOGICAL_HEIGHT 500

ClassicGameWidget::ClassicGameWidget(AbstractRule::Gesture gesture) :
    frameCount(0),
    noSolutionCount(0)
{
  if (gesture == AbstractRule::Swap)
    rule = new SwapClassicGameRule();
  else
    rule = new RotateClassicGameRule();
  gameboardInfo = new ThirtySevenGameBoardInfo();

  OtherGameRecord *record = OtherGameInit::loadOtherGame(getIndex());

  //ClassicGameWidgetSavedInfo savedInfo = readSaved();
  controller = new CoreController(rule, gameboardInfo, record->balls);
  controller->fillAllBlanks();
  for (int i = 0;i < 100;++i)
    controller->advance();
  gestureController = new GestureController(rule, gameboardInfo, controller);

  effectPainter = new EffectPainter(gameboardInfo);

  hightestScore = new IntegerItem();
  hightestScore->setPos(QPointF(0.1, 0.1));
  hightestScore->setValue(record->highestScore);
  hightestScore->setHint("Highest Score");
  myItems.push_back(hightestScore);

  currentLevel = new IntegerItem();
  currentLevel->setPos(QPointF(0.1, 0.15));
  currentLevel->setValue(record->currentLevel);
  currentLevel->setHint("Current Level");
  myItems.push_back(currentLevel);

  progressBar = new VerticalProgressBarItem();
  progressBar->setPos(QPointF(0.1, 0.2));
  progressBar->setCurrent(record->currentScore);
  progressBar->setMin(record->minScore);
  progressBar->setMax(record->maxScore);
  myItems.push_back(progressBar);

  flame = new FlameItem();
  flame->setPos(QPointF(0.1, 0.3));
  flame->setCurrent(record->flame);
  myItems.push_back(flame);

  star = new StarItem();
  star->setPos(QPointF(0.1, 0.5));
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

  itemAtPressPos = NULL;

  delete record;

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

void ClassicGameWidget::makePixmap(QPixmap& pixmap, int width, int height)
{
  makeBasicPixmap(pixmap, width, height);
  addEffect(pixmap, width, height);
}

ClassicGameWidget::~ClassicGameWidget()
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

//void ClassicGameWidget::init() //
//}

void ClassicGameWidget::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
  pixmap = QPixmap(width, height);
  pixmap.fill(Qt::black);
  QPainter *painter = new QPainter(&pixmap);
  Ball **balls = controller->balls;
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

void ClassicGameWidget::addEffect(QPixmap& pixmap, int width, int height)
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

QPointF ClassicGameWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * gameboardInfo->width(),
                 yRate * gameboardInfo->height());
}

//ClassicGameWidgetSavedInfo ClassicGameWidget::readSaved()
//{

//}

void ClassicGameWidget::showHint()
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

void ClassicGameWidget::gameOver()
{
  OtherGameInit::clearGame(getIndex());
  OtherGameInit::testHighest(getIndex(), progressBar->getCurrent());
  GameOverWidget *w = new GameOverWidget(getIndex(), progressBar->getCurrent());
  emit giveControlTo(w, true);
  delete this;
}

void ClassicGameWidget::quitGame()
{
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

void ClassicGameWidget::dealPressed(QPointF mousePos, Qt::MouseButton button)
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

void ClassicGameWidget::dealMoved(QPointF mousePos, Qt::MouseButton button)
{
  currentPos = mousePos;
  effectPainter->clearUserMovingEliminationHints();
  gestureController->dealMoved(mousePos);
}

void ClassicGameWidget::dealReleased(QPointF mousePos, Qt::MouseButton button)
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
      int score = qMax(progressBar->getCurrent() - 10, progressBar->getMin());
      progressBar->setCurrent(score);
      showHint();
    }
    else if (itemAtPressPos == resetItem &&
             distanceOfTwoPoints(mousePos,
                                 toScene(resetItem->getPos().x(),
                                         resetItem->getPos().y())) < 30)
    {
      ResetWidget *w = new ResetWidget();
      connect(w, SIGNAL(confirm()), this, SLOT(reset()));
      emit giveControlTo(w, false);
    }
    else if (itemAtPressPos == exitItem &&
             distanceOfTwoPoints(mousePos,
                                 toScene(exitItem->getPos().x(),
                                         exitItem->getPos().y())) < 30)
      quitGame();
  }

  effectPainter->clearUserMovingEliminationHints();
  itemAtPressPos = NULL;
  gestureController->dealReleased(mousePos);
}

void ClassicGameWidget::advance()
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
  controller->advance();
  if (noSolutionCount > 30)
    gameOver();
//  effectPainter->advance();
}

void ClassicGameWidget::dealStableEliminate(Connections connections)
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
  if (progressBar->getCurrent() > hightestScore->getValue())
  {
    OtherGameInit::testHighest(getIndex(), progressBar->getCurrent());
    hightestScore->setValue(progressBar->getCurrent());
  }
}

void ClassicGameWidget::dealUserMovingEliminate(Connections connections)
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

void ClassicGameWidget::nextStage()
{
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

  ClassicGameWidget *nextStage;
  if (rule->gestureAllowed(AbstractRule::Swap))
    nextStage = new ClassicGameWidget(AbstractRule::Swap);
  else
    nextStage = new ClassicGameWidget(AbstractRule::Rotate);
  emit giveControlTo(nextStage, true);
  delete this;
}

int ClassicGameWidget::getIndex()
{
  return 0 + (rule->gestureAllowed(AbstractRule::Rotate) ? 1 : 0);
}

void ClassicGameWidget::reset()
{
  OtherGameInit::clearGame(getIndex());
  ClassicGameWidget *resetGame;
  if (rule->gestureAllowed(AbstractRule::Swap))
    resetGame = new ClassicGameWidget(AbstractRule::Swap);
  else
    resetGame = new ClassicGameWidget(AbstractRule::Rotate);
  emit giveControlTo(resetGame, true);
  delete this;
}
