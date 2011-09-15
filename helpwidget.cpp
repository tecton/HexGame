#include "helpwidget.h"

#include <QPainter>
#include "basicpainter.h"
#include "gamecommonitems.h"
#include "gamemath.h"
#include "othergameinit.h"
#include "abstractrule.h"
#include "pixmapoperations.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

HelpWidget::HelpWidget() :
    dy(0)
{
  // Init he pixmap
  p = QPixmap(":/images/helpwidget/helpwidget.png");

  // Create the item and initialize it
  exitItem = new ExitItem();
  exitItem->setPos(QPointF(0.5, 0.95));
  myItems.push_back(exitItem);

  // No items was chosen
  itemAtPressPos = NULL;
}

void HelpWidget::makePixmap(QPixmap& pixmap, int width, int height)
{
  makeBasicPixmap(pixmap, width, height);
  addEffect(pixmap, width, height);
}

void HelpWidget::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
  pixmap = QPixmap(width, height);

  // Fill the pixmap with black background
  pixmap.fill(Qt::black);

  // Get the painter
  QPainter *painter = new QPainter(&pixmap);

  // The scale of the pixmap
  double scale;

  // Calculate the scale
  scale = 1.0 * width / p.width();

  // Calculate the deviation in y direction
  dy = qMax(qMin(dy, p.height() * scale - height), 0.0);

  // Draw the pixmap
  painter->drawPixmap(0,
                      -dy,
                      width,
                      p.height() * scale,
                      p
                      );

  // Paint the items
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           0);

  // End the paint and release the space
  painter->end();
  delete painter;
}

void HelpWidget::addEffect(QPixmap& pixmap, int width, int height){}

QPointF HelpWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}

void HelpWidget::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  // Choose the correct item at press position
  lastPos = mousePos;
  if (distanceOfTwoPoints(mousePos,
                          toScene(exitItem->getPos().x(),
                                  exitItem->getPos().y())) < 50)
    itemAtPressPos = exitItem;
}

void HelpWidget::dealMoved(QPointF mousePos, Qt::MouseButton button)
{
  // Recalculate the dy
  dy = dy + lastPos.y() - mousePos.y();

  // Record the position of the mouse
  lastPos = mousePos;
}

void HelpWidget::dealReleased(QPointF mousePos, Qt::MouseButton button)
{
  if (distanceOfTwoPoints(mousePos,
                          toScene(exitItem->getPos().x(),
                                  exitItem->getPos().y())) < 50 &&
      itemAtPressPos == exitItem)
  {
    // Exit
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }

  itemAtPressPos = NULL;
}

HelpWidget::~HelpWidget()
{
  // Release the space
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
}
