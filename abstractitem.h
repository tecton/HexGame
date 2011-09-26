#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

// File must include
#include <QPointF>
#include "gamemath.h"

// Forward declaration
class QPainter;

/**
 * @brief An abstract class of an item.
 * It should have the ability to paint itself.
 */
class AbstractItem
{
public:
  /**
   *@brief Paint the item.
   *
   * Some detailed comment.
   *@param painter The QPainter to do the paint.
   *@param width The width of the whole QPaintDevice.
   *@param height The height of the whole QPaintDevice.
   *@param frame The index of the frame to show
   *       (An item may have lots of frames which will realize an animation).
   */
  virtual void paint(QPainter *painter, int width, int height, int frame)=0;

  /**
   *@brief Set the position of the item(The position is relative one,
   * NOT absolute one!).
   */
  void setPos(QPointF pos)
  {position = pos;}

  /**
   *@brief Get the position of the item(The position is relative one,
   * NOT absolute one!).
   */
  QPointF getPos()
  {return position;}

  /**
   *@brief Judget whether a mouse is in the item(The position is absolute one,
   * NOT relative one!).
   */
  virtual bool in(QPointF mousePos, int width, int height)=0;

private:
  // The position of the item
  // (The position is relative one, NOT absolute one!)
  QPointF position;
};

/**
 * @brief An abstract class of an item which is rectangle.
 */
class AbstractRectItem : public AbstractItem
{
public:
  virtual void paint(QPainter *painter, int width, int height, int frame)=0;

  virtual bool in(QPointF mousePos, int windowWidth, int windowHeight)
  {
    QPointF centerPos = getPos();
    centerPos.setX(centerPos.x() * windowWidth);
    centerPos.setY(centerPos.y() * windowHeight);
    return mousePos.x() - centerPos.x() < width() &&
           mousePos.x() - centerPos.x() > -width() &&
           mousePos.y() - centerPos.y() < height() &&
           mousePos.y() - centerPos.y() > -height();
  }

  virtual double width()=0;
  virtual double height()=0;
};

/**
 * @brief An abstract class of an item which is circle.
 */
class AbstractCircleItem : public AbstractItem
{
public:
  virtual void paint(QPainter *painter, int width, int height, int frame)=0;

  virtual bool in(QPointF mousePos, int windowWidth, int windowHeight)
  {
    QPointF centerPos = getPos();
    centerPos.setX(centerPos.x() * windowWidth);
    centerPos.setY(centerPos.y() * windowHeight);
    return distanceOfTwoPoints(mousePos, centerPos) <= r();
  }

  virtual double r()=0;
};

#endif // ABSTRACTITEM_H
