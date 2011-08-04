#include "gamemath.h"


// Returns the angle of the point acording to the origin
// Result should be in [0, 2*PI)
qreal angle(QPointF point, QPointF origin)
{
  qreal dx = point.x() - origin.x();
  qreal dy = origin.y() - point.y();
  qreal result = qAtan(qAbs(dy / dx));
  if (dx < 0 && dy >= 0)
    return PI - result;
  if (dx <= 0 && dy < 0)
    return PI + result;
  if (dx > 0 && dy < 0)
    return 2 * PI - result;
  if (result != result) // NaN !!!
    return 0;
  return result;
}

// Calculate the position acording to the angle, radius and origin
QPointF calculatePosition(qreal a, qreal r, QPointF origin)
{
  qreal dx = qCos(a) * r;
  qreal dy = -qSin(a) * r;
  qreal x = dx + origin.x();
  qreal y = dy + origin.y();
  return QPointF(x, y);
}

// When the translate path is ^, suppose that the distance between begin
// position and end position is 1, inputs the x position, returns the y
// position(I should draw a picture to show how it works-.-)
qreal bridgeY(qreal bridgeX)
{
  if (bridgeX <= 0 || bridgeX >= 1)
    return 0;
  qreal dx = qAbs(0.5 - bridgeX);
  return qSqrt(1 - qPow(dx, 2)) - 0.8660;
}

// Distance of two points
qreal distanceOfTwoPoints(QPointF p1, QPointF p2)
{
  return qSqrt(qPow(p1.x() - p2.x(), 2) + qPow(p1.y() - p2.y(), 2));
}

// Distance from the center with the angle, helps to calculate the distance
// from the center and locate the balls on the hexagon
qreal distanceFromTheCenterWithTheAngle(qreal angle, qreal maxR)
{
  while (angle < 0)
    angle += PI / 3;
  while (angle >= PI / 3)
    angle -= PI / 3;
  return maxR * 2 *
         qSin(PI / 3) /
         qSin(2 * PI / 3 - angle);
}