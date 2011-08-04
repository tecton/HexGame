#ifndef GAMEITEMS_H
#define GAMEITEMS_H

#include "abstractitem.h"

#include <QPointF>

class AbstractProgressBarItem : public AbstractItem
{
public:
  // The virtual function to be overloaded
  virtual void paint(QPainter *painter, int width, int height, int frame)=0;

  inline int getMin()
  {return min;}

  inline int getMax()
  {return max;}

  inline int getCurrent()
  {return current;}

  inline void setMin(int v)
  {min = v;}

  inline void setMax(int v)
  {max = v;}

  inline void setCurrent(int v)
  {current = v;}

private:
  int min;
  int max;
  int current;

};

class VerticalProgressBarItem : public AbstractProgressBarItem
{
public:
  VerticalProgressBarItem();

  // The virtual function to be overloaded
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

class ReturnToMainMenuButtonItem : public AbstractItem
{
public:
  // The virtual function to be overloaded
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

#endif // GAMEITEMS_H
