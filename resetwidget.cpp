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
  // Create the items and initialize them
  confirmItem = new ConfirmItem();
  confirmItem->setPos(QPointF(0.3, 0.5));
  myItems.push_back(confirmItem);

  cancelItem = new CancelItem();
  cancelItem->setPos(QPointF(0.7, 0.5));
  myItems.push_back(cancelItem);

  // No items was chosen
  itemAtPressPos = NULL;
}

void ResetWidget::makePixmap(
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

void ResetWidget::makeBasicPixmap(
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
#else
  painter->fillRect(0,0,width,height,QColor(0,0,0));
#endif

  // Paint the items
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           0);

#ifdef USE_PIXMAP
  // End the paint and release the space
  painter->end();
  delete painter;
#endif
}

void ResetWidget::addEffect(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height){}

QPointF ResetWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}

void ResetWidget::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  // Choose the correct item at press position
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
    // Exit
    emit giveControlTo(NULL, true);

    // Emit correct signal
    emit confirm();
    delete this;
    return;
  }
  else if (distanceOfTwoPoints(mousePos,
                               toScene(cancelItem->getPos().x(),
                                       cancelItem->getPos().y())) < 50 &&
           itemAtPressPos == cancelItem)
  {
    // Exit
    emit giveControlTo(NULL, true);

    // Emit correct signal
    emit cancel();
    delete this;
    return;
  }

  itemAtPressPos = NULL;
}

ResetWidget::~ResetWidget()
{
  // Release the space
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
}
