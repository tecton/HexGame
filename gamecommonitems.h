#ifndef GAMECOMMONITEMS_H
#define GAMECOMMONITEMS_H

#include "abstractitem.h"

class AbstractProgressBarItem : public AbstractItem
{
public:
  void setMin(int v)
  {min = v;}

  int getMin()
  {return min;}

  void setMax(int v)
  {max = v;}

  int getMax()
  {return max;}

  void setCurrent(int v)
  {current = v;}

  int getCurrent()
  {return current;}


  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame)=0;

private:
  int min;
  int max;
  int current;
};

class VerticalProgressBarItem : public AbstractProgressBarItem
{
public:
  VerticalProgressBarItem();

  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);

};

#endif // GAMECOMMONITEMS_H
