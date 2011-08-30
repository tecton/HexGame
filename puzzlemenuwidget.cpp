#include "puzzlemenuwidget.h"

#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include "basicpainter.h"
#include "gamemath.h"
#include "puzzlemenuitems.h"
#include "stagemenuwidget.h"

#define LOGICAL_WIDTH  800
#define LOGICAL_HEIGHT 500


PuzzleMenuWidget::PuzzleMenuWidget() :
    frameCount(0)
{
  exchangeItem = new PuzzleMenuExchangeItem();
  exchangeItem->setPos(QPointF(0.2, 0.45));

  uniteItem = new PuzzleMenuUniteItem();
  uniteItem->setPos(QPointF(0.5, 0.45));

  lockItem = new PuzzleMenuLockItem();
  lockItem->setPos(QPointF(0.8, 0.45));

  myItems.push_back(exchangeItem);
  myItems.push_back(uniteItem);
  myItems.push_back(lockItem);

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
  if (distanceOfTwoPoints(mousePos,
                          toScene(exchangeItem->getPos().x(),
                                  exchangeItem->getPos().y())) < 80)
  {
    AbstractStageMenuWidget *exchangeMenu = new ExchangeStageMenuWidget(0);
    emit giveControlTo(exchangeMenu, false);
  }
  if (distanceOfTwoPoints(mousePos,
                          toScene(uniteItem->getPos().x(),
                                  uniteItem->getPos().y())) < 80)
  {
    AbstractStageMenuWidget *uniteMenu = new UniteStageMenuWidget(0);
    emit giveControlTo(uniteMenu, false);
  }
  if (distanceOfTwoPoints(mousePos,
                          toScene(lockItem->getPos().x(),
                                  lockItem->getPos().y())) < 80)
  {
    AbstractStageMenuWidget *lockMenu = new LockStageMenuWidget(0);
    emit giveControlTo(lockMenu, false);
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
