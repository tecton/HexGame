#ifndef GAMEMATH_H
#define GAMEMATH_H

#include <qmath.h>
#include <QPointF>

// PI
#define PI                                      3.14159265358979

// Rad to angle
inline qreal radiansToAngle(qreal r)
{ return r * 180 / PI;}

// Angle to rad
inline qreal angleToRadians(qreal a)
{ return a / 180 * PI;}

// Returns the angle of the point acording to the origin
// Result should be in [0, 2*PI)
qreal angle(QPointF point, QPointF origin);

// Calculate the position acording to the angle, radius and origin
QPointF calculatePosition(qreal a, qreal r, QPointF origin);

// When the translate path is ^, suppose that the distance between begin
// position and end position is 1, inputs the x position, returns the y
// position(I should draw a picture to show how it works-.-)
qreal bridgeY(qreal bridgeX);

// Distance of two points
qreal distanceOfTwoPoints(QPointF p1, QPointF p2);

// Distance from the center with the angle, helps to calculate the distance
// from the center and locate the balls on the hexagon
qreal distanceFromTheCenterWithTheAngle(qreal angle, qreal maxR);

#endif // GAMEMATH_H
