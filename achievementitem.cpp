#include "achievementitem.h"

#include <QPainter>
#include <QColor>
#include "achievements.h"

#define LOGICAL_WIDTH    1024
#define LOGICAL_HEIGHT   600
#define ITEM_WIDTH       400.0
#define ITEM_HEIGHT      150.0

#define HALF_WIDTH_RATE  (ITEM_WIDTH / 2 / LOGICAL_WIDTH)
#define HALF_HEIGHT_RATE (ITEM_HEIGHT / 2 / LOGICAL_HEIGHT)

double AbstractAchievementItem::width()
{
  return ITEM_WIDTH;
}

double AbstractAchievementItem::height()
{
  return ITEM_HEIGHT;
}

void AbstractAchievementItem::paintDescription(QPainter *painter,
                                               QRectF rect,
                                               int frame)
{
  painter->setPen(QColor(255,0,0));
  painter->drawRect(rect);
  painter->drawText(rect, Qt::AlignCenter, getDescription());
}

FlameGetItem::FlameGetItem(int theLevel, int theCurrent) :
    level(theLevel),
    current(theCurrent)
{
  static int asdf = 0;
  ++asdf;
  current = asdf;

  next = kFlameGetStage[level];
  if (next != -1)
    description = QObject::tr("Level %1\nYou've got %2 flames\nGet %3 flames to reach next stage\n").arg(level).arg(current).arg(next-current);
  else
    description = QObject::tr("Level %1\nIt's already the MAX LEVEL").arg(level);
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
