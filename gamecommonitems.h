#ifndef GAMECOMMONITEMS_H
#define GAMECOMMONITEMS_H

#include "abstractitem.h"
#include <QString>

class AbstractGameBoardInfo;
class EffectPainter;

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

class AbstractBonusItem : public AbstractItem
{
public:
  inline void setCurrent(int v)
  {
    if (v <= max && v >= 0)
      count = v;
  }

  inline int getCurrent()
  {return count;}

  inline void setMax(int v)
  {
    if (v >= 0)
      max = v;
    if (count > max)
      count = max;
  }

  inline int getMax()
  {return max;}

  inline void addOne()
  {
    if (count < max)
      ++count;
  }

  inline void minusOne()
  {
    if (count > 0)
      --count;
  }

  inline bool notEmpty()
  {return count != 0;}

  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame)=0;

  virtual void paintLocatingIcon(QPainter *painter,
                                 QPointF pos,
                                 int frame)=0;

  virtual void paintInfluencedArea(int index,
                                   AbstractGameBoardInfo *gameBoard,
                                   EffectPainter *effectPainter,
                                   int frame)=0;

private:
  int max;
  int count;
};

class FlameItem : public AbstractBonusItem
{
public:
  FlameItem();

  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);

  virtual void paintLocatingIcon(QPainter *painter,
                                 QPointF pos,
                                 int frame);

  virtual void paintInfluencedArea(int index,
                                   AbstractGameBoardInfo *gameBoard,
                                   EffectPainter *effectPainter,
                                   int frame);
};

class StarItem : public AbstractBonusItem
{
public:
  StarItem();

  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);

  virtual void paintLocatingIcon(QPainter *painter,
                                 QPointF pos,
                                 int frame);

  virtual void paintInfluencedArea(int index,
                                   AbstractGameBoardInfo *gameBoard,
                                   EffectPainter *effectPainter,
                                   int frame);
};

class HintItem : public AbstractItem
{
public:
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);
};

class ExitItem : public AbstractItem
{
public:
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);
};

class ResetItem : public AbstractItem
{
public:
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);
};

class PauseItem : public AbstractItem
{
public:
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);
};

class IntegerItem : public AbstractItem
{
public:
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);

  inline void setHint(QString str)
  {hint = str;}

  inline QString getHint()
  {return hint;}

  inline void setValue(int v)
  {value = v;}

  inline int getValue()
  {return value;}

private:
  QString hint;
  int value;
};

class StringItem : public AbstractItem
{
public:
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);

  inline void setHint(QString str)
  {hint = str;}

  inline QString getHint()
  {return hint;}

private:
  QString hint;
  int value;
};

class ConfirmItem : public AbstractItem
{
public:
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);
};

class CancelItem : public AbstractItem
{
public:
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);
};


#endif // GAMECOMMONITEMS_H
