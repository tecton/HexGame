/**
 *@brief Several functions which helps to calculate.
 */

#ifndef GAMEMATH_H
#define GAMEMATH_H

#include <qmath.h>
#include <QPointF>

// PI
#ifndef PI
#define PI                                      3.14159265358979
#endif

/**
 *@brief Returns the angle of the point acording to the origin.
 *
 *Up will be the positive direction of y.
 *Right will be the positive direction of x.
 *The angle will be counterclockwise.
 *@param point The point.
 *@param origin The origin.
 *@return The angle(in [0, 2*PI)).
 */
qreal angle(QPointF point, QPointF origin);

/**
 *@brief Returns the position acording to the angle, radius and origin.
 *
 *Up will be the positive direction of y.
 *Right will be the positive direction of x.
 *The angle will be counterclockwise.
 *@param a The angle.
 *@param r The radius.
 *@param origin The origin.
 *@return The position.
 */
QPointF calculatePosition(qreal a, qreal r, QPointF origin);

/**
 *@brief Returns the scale in Y direction in a curve.
 *
 * When the translate path is ^, suppose that the distance between begin
 * position and end position is 1, inputs the x position, returns the y
 * position(I should draw a picture to show how it works-.-)
 *@param bridgeX The position in X direction.
 *@return The position in Y direction.
 */
qreal bridgeY(qreal bridgeX);

/**
 *@brief Distance of two points.
 *
 *@param p1 One point.
 *@param p2 Another point.
 *@return Distance of two points.
 */
qreal distanceOfTwoPoints(QPointF p1, QPointF p2);

/**
 *@brief Distance from the center with the angle in a hexagon.
 *
 * Helps to locate the balls.
 *@param angle The angle.
 *@param maxR The max radius.
 *@return Distance of two points.
 */
qreal distanceFromTheCenterWithTheAngle(qreal angle, qreal maxR);

/**
 *@brief Calculate a new position from the original position and scale rate in
 * 2 directions.
 *
 * Point(0,0) will be the origin.
 *@param originalPos The original position.
 *@param xRate The scale in X direction.
 *@param yRate The scale in Y direction.
 *@return The new point in the new scale.
 */
QPointF scale(QPointF originalPos, double xRate, double yRate);

#endif // GAMEMATH_H
