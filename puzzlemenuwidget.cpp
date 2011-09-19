#include "puzzlemenuwidget.h"

#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include "basicpainter.h"
#include "gamemath.h"
#include "puzzlemenuitems.h"
#include "stagemenuwidget.h"
#include "gamecommonitems.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

PuzzleMenuWidget::PuzzleMenuWidget() :
    frameCount(0)
{
  // initialize four items and set position
  exchangeItem = new PuzzleMenuExchangeItem();
  exchangeItem->setPos(QPointF(0.2, 0.45));

  uniteItem = new PuzzleMenuUniteItem();
  uniteItem->setPos(QPointF(0.5, 0.45));

  lockItem = new PuzzleMenuLockItem();
  lockItem->setPos(QPointF(0.8, 0.45));

  exitItem = new ExitItem();
  exitItem->setPos(QPointF(0.5, 0.8));

  // collect them
  myItems.push_back(exchangeItem);
  myItems.push_back(uniteItem);
  myItems.push_back(lockItem);
  myItems.push_back(exitItem);

  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
  t->start();
}

void PuzzleMenuWidget::makePixmap(QPixmap& pixmap, int width, int height)
{
  makeBasicPixmap(pixmap, width, height);
  addEffect(pixmap, width, height);
}

void PuzzleMenuWidget::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
  pixmap = QPixmap(width, height);
  pixmap.fill(Qt::black);
  QPainter *painter = new QPainter(&pixmap);
  BasicPainter::paintBackGround(BasicPainter::PuzzleMenu,
                                painter,
                                width,
                                height,
                                frameCount);
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           frameCount);

  painter->end();
  delete painter;
}

void PuzzleMenuWidget::addEffect(QPixmap& pixmap, int width, int height)
{

}

QPointF PuzzleMenuWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}

void PuzzleMenuWidget::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  if (button == Qt::RightButton)
  {
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }
  // determine the position where mouse down happens and init
  // corresponding game
  if (distanceOfTwoPoints(mousePos,
                          toScene(exchangeItem->getPos().x(),
                                  exchangeItem->getPos().y())) < 80)
  {
    AbstractStageMenuWidget *exchangeMenu = new ExchangeStageMenuWidget(0);
    emit giveControlTo(exchangeMenu, false);
  }
  else if (distanceOfTwoPoints(mousePos,
                               toScene(uniteItem->getPos().x(),
                                       uniteItem->getPos().y())) < 80)
  {
    AbstractStageMenuWidget *uniteMenu = new UniteStageMenuWidget(0);
    emit giveControlTo(uniteMenu, false);
  }
  else if (distanceOfTwoPoints(mousePos,
                               toScene(lockItem->getPos().x(),
                                       lockItem->getPos().y())) < 80)
  {
    AbstractStageMenuWidget *lockMenu = new LockStageMenuWidget(0);
    emit giveControlTo(lockMenu, false);
  }
  else if (distanceOfTwoPoints(mousePos,
                               toScene(exitItem->getPos().x(),
                                       exitItem->getPos().y())) < 80)
  {
    // exit and give out control
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }
}

void PuzzleMenuWidget::dealMoved(QPointF mousePos, Qt::MouseButton button)
{

}

void PuzzleMenuWidget::dealReleased(QPointF mousePos, Qt::MouseButton button)
{

}

void PuzzleMenuWidget::advance()
{
  ++frameCount;
  frameCount = frameCount % 32;
  //effectPainter->advance();
}

PuzzleMenuWidget::~PuzzleMenuWidget()
{
  t->stop();
  delete t;
  delete exchangeItem;
  delete uniteItem;
}
