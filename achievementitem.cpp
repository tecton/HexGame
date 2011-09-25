#include "achievementitem.h"

#include <QPainter>
#include <QColor>

#define LOGICAL_WIDTH    1024
#define LOGICAL_HEIGHT   600
#define ITEM_WIDTH       400.0
#define ITEM_HEIGHT      150.0

#define HALF_WIDTH_RATE  (ITEM_WIDTH / 2 / LOGICAL_WIDTH)
#define HALF_HEIGHT_RATE (ITEM_HEIGHT / 2 / LOGICAL_HEIGHT)

bool AbstractAchievementItem::in(QPointF mousePos)
{
  return mousePos.x() - getPos().x() < HALF_WIDTH_RATE &&
         mousePos.x() - getPos().x() > -HALF_WIDTH_RATE &&
         mousePos.y() - getPos().y() < HALF_HEIGHT_RATE &&
         mousePos.y() - getPos().y() > -HALF_HEIGHT_RATE;
}

FlameGetItem::FlameGetItem(int theLevel, int theCurrent) :
    level(theLevel),
    current(theCurrent)
{
}

void FlameGetItem::paint(QPainter *painter,
                         int width,
                         int height,
                         int frame)
{
  // Calculate values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  double xRate = 1.0 * width / LOGICAL_WIDTH;
  double yRate = 1.0 * height / LOGICAL_HEIGHT;
  painter->setPen(QColor(255,0,0));
  painter->drawText(x, y, "BLABLABLA");

}

void FlameGetItem::paintDescription(QPainter *painter,
                                    QRectF rect,
                                    int frame)
{
  painter->setPen(QColor(255,0,0));
  painter->drawRect(rect);
}
