#include "puzzlemenuwidget.h"

#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include "basicpainter.h"
#include "gamemath.h"
#include "puzzlemenuitems.h"
#include "stagemenuwidget.h"

#include <QMessageBox>

#define MAIN_MENU_LOGICAL_WIDTH  800
#define MAIN_MENU_LOGICAL_HEIGHT 500


PuzzleMenuWidget::PuzzleMenuWidget() :
    frameCount(0)
{
  exchangeItem = new PuzzleMenuExchangeItem();
  exchangeItem->setPos(QPointF(0.3, 0.3));

  uniteItem = new PuzzleMenuUniteItem();
  uniteItem->setPos(QPointF(0.7,0.3));

  myItems.push_back(exchangeItem);
  myItems.push_back(uniteItem);

  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
  t->start();
}

void PuzzleMenuWidget::makePixmap(QPixmap& pixmap, int width, int height)
{
  makeBasicPixmap(pixmap, width, height);
//  QMessageBox::critical(0,"","Basic made");
  addEffect(pixmap, width, height);
}

void PuzzleMenuWidget::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
  pixmap = QPixmap(width, height);
  pixmap.fill(Qt::black);
  QPainter *painter = new QPainter(&pixmap);
  //QMessageBox::critical(0,"","Try to paint items");
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           frameCount);

//  QMessageBox::critical(0,"","Items painted");
  painter->end();
  delete painter;
}

void PuzzleMenuWidget::addEffect(QPixmap& pixmap, int width, int height)
{

}

QPointF PuzzleMenuWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * MAIN_MENU_LOGICAL_WIDTH,
                 yRate * MAIN_MENU_LOGICAL_HEIGHT);
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
                          QPointF(0.3 * MAIN_MENU_LOGICAL_WIDTH,
                                  0.3 * MAIN_MENU_LOGICAL_HEIGHT)) < 50)
  {
    AbstractStageMenuWidget *exchangeMenu = new ExchangeStageMenuWidget();
    emit giveControlTo(exchangeMenu, false);
  }
  if (distanceOfTwoPoints(mousePos,
                          QPointF(0.7 * MAIN_MENU_LOGICAL_WIDTH,
                                  0.3 * MAIN_MENU_LOGICAL_HEIGHT)) < 50)
  {
    AbstractStageMenuWidget *uniteMenu = new UniteStageMenuWidget();
    emit giveControlTo(uniteMenu, false);
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
