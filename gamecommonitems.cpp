#include "gamecommonitems.h"

#include <QPainter>

VerticalProgressBarItem::VerticalProgressBarItem()
{
  setMin(0);
  setMax(30);
  setCurrent(0);
}


void VerticalProgressBarItem::paint(QPainter *painter,
                                    int width,
                                    int height,
                                    int frame)
{
  painter->setPen(QColor(255,255,255,255));
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  int percentage = (getCurrent() - getMin()) * 100 / (getMax() - getMin());
  painter->drawText(QPointF(x, y),
                    QObject::tr("%1 % (%2-%3)").
                    arg(percentage).
                    arg(getCurrent()).
                    arg(getMax()));
}
