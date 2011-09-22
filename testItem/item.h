#ifndef ITEM_H
#define ITEM_H

// File must include
#include <QPointF>

// Forward declaration
class QPainter;

class Item
{
public:
  Item(int s = 1,
       QPointF pos = QPointF(0, 0),
       bool l = false,
       bool a = false);

  virtual void paint(QPainter *painter, int width, int height, int frame);

  void setPos(QPointF pos)
  {position = pos;}

  QPointF getPos()
  {return position;}

  void setLocked(bool v)
  {locked = v;}

  bool getLocked()
  {return locked;}

  void setAdvance(bool v)
  {advance = v;}

  bool getAdvance()
  {return advance;}

private:
  int stage;

  QPointF position;

  bool locked;

  bool advance;
};

#endif // ITEM_H
