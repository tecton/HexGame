#include "resetwidget.h"

#include <QPainter>
#include <QPixmap>
#include "basicpainter.h"
#include "gamecommonitems.h"
#include "gamemath.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

ResetWidget::ResetWidget()
{
  confirmItem = new ConfirmItem();
  confirmItem->setPos(QPointF(0.3, 0.5));
  myItems.push_back(confirmItem);

  cancelItem = new CancelItem();
  cancelItem->setPos(QPointF(0.7, 0.5));
  myItems.push_back(cancelItem);

  itemAtPressPos = NULL;
}

void ResetWidget::makePixmap(QPixmap& pixmap, int width, int height)
{
  makeBasicPixmap(pixmap, width, height);
  addEffect(pixmap, width, height);
}

//  void ResetWidget::init();
void ResetWidget::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
  pixmap = QPixmap(width, height);
  pixmap.fill(Qt::black);
  QPainter *painter = new QPainter(&pixmap);
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           0);

  painter->end();
  delete painter;
}

void ResetWidget::addEffect(QPixmap& pixmap, int width, int height){}

QPointF ResetWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}

void ResetWidget::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  if (distanceOfTwoPoints(mousePos,
                          toScene(confirmItem->getPos().x(),
                                  confirmItem->getPos().y())) < 50)
    itemAtPressPos = confirmItem;
  else if (distanceOfTwoPoints(mousePos,
                               toScene(cancelItem->getPos().x(),
                                       cancelItem->getPos().y())) < 50)
    itemAtPressPos = cancelItem;
}

void ResetWidget::dealMoved(QPointF mousePos, Qt::MouseButton button){}

void ResetWidget::dealReleased(QPointF mousePos, Qt::MouseButton button)
{
  if (distanceOfTwoPoints(mousePos,
                          toScene(confirmItem->getPos().x(),
                                  confirmItem->getPos().y())) < 50 &&
      itemAtPressPos == confirmItem)
  {
    emit giveControlTo(NULL, true);
    emit confirm();
    delete this;
    return;
  }
  else if (distanceOfTwoPoints(mousePos,
                               toScene(cancelItem->getPos().x(),
                                       cancelItem->getPos().y())) < 50 &&
           itemAtPressPos == cancelItem)
  {
    emit giveControlTo(NULL, true);
    emit cancel();
    delete this;
    return;
  }

  itemAtPressPos = NULL;
}

ResetWidget::~ResetWidget()
{
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
}
