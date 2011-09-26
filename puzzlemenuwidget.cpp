#include "puzzlemenuwidget.h"

#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include "basicpainter.h"
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

  exitItem = new ButtonItem("Exit");
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

void PuzzleMenuWidget::makePixmap(
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

void PuzzleMenuWidget::makeBasicPixmap(
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

#ifdef USE_PIXMAP
  painter->end();
  delete painter;
#endif
}

void PuzzleMenuWidget::addEffect(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height)
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
  if (exchangeItem->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
  {
    AbstractStageMenuWidget *exchangeMenu = new ExchangeStageMenuWidget(0);
    emit giveControlTo(exchangeMenu, false);
    return;
  }
  else if (uniteItem->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
  {
    AbstractStageMenuWidget *uniteMenu = new UniteStageMenuWidget(0);
    emit giveControlTo(uniteMenu, false);
    return;
  }
  else if (lockItem->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
  {
    AbstractStageMenuWidget *lockMenu = new LockStageMenuWidget(0);
    emit giveControlTo(lockMenu, false);
    return;
  }
  else if (exitItem->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
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
