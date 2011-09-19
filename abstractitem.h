/*
** Abstract class of an item.
** It should have the ability to paint itself.
*/

#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

// File must include
#include <QPointF>

// Forward declaration
class QPainter;

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
   *@exception std::out_of_range [exception's comment.]
   *@return [return's comment.]
   */
  virtual void paint(QPainter *painter, int width, int height, int frame)=0;

  //@{
  /** Set and get the position of the item.
   *  (The position is relative one, NOT absolute one!)
   */
  void setPos(QPointF pos)
  {position = pos;}

  QPointF getPos()
  {return position;}
  //@}

private:
  // The position of the item
  // (The position is relative one, NOT absolute one!)
  QPointF position;
};

#endif // ABSTRACTITEM_H
