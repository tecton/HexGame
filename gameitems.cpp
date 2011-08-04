#include "gameitems.h"

#include <QPainter>
#include <QColor>

void VerticalProgressBarItem::paint(QPainter *painter,
                                    int width,
                                    int height,
                                    int frame)
{
  painter->setPen(QColor(255,255,255,255));
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  int percentage = 100 * (getCurrent() - getMin()) / (getMax() - getMin());
  painter->drawText(QPointF(x, y), QObject::tr("%1 % (%2-%3)").arg(percentage).arg(getCurrent()).arg(getMax()));

}

VerticalProgressBarItem::VerticalProgressBarItem()
{
  setPos(QPointF(0, 0));
  setMin(0);
  setMax(20);
  setCurrent(0);
}

