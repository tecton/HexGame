/*
** Classes of items may be used in a game.
*/

#ifndef GAMECOMMONITEMS_H
#define GAMECOMMONITEMS_H

// File must include
#include "abstractitem.h"
#include <QString>
#include <QPixmap>

// Forward declaration
class AbstractGameBoardInfo;
class EffectPainter;

// An abstract class of an item of a progress bar
class AbstractProgressBarItem : public AbstractItem
{
public:
  // Functions to set and get the 3 values
  inline void setMin(int v)
  {min = v;}

  inline int getMin()
  {return min;}

  inline void setMax(int v)
  {max = v;}

  inline int getMax()
  {return max;}

  inline void setCurrent(int v)
  {current = v;}

  inline int getCurrent()
  {return current;}

  // Function most overloaded(still a pure virtual one)
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame)=0;

private:
  // 3 key values of a progress bar
  int min;
  int max;
  int current;
};

// A class of an item of a vertical progress bar
class VerticalProgressBarItem : public AbstractProgressBarItem
{
public:
  // Constructor
  VerticalProgressBarItem();

  // Function most overloaded
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);

private:
  // Pixmaps to do the paint
  QPixmap background;
  QPixmap foreground;
};

// An abstract class of an item of a bonus
class AbstractBonusItem : public AbstractItem
{
public:
  // Functions to set and get the 2 values
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

  // Add one and minus one, may be used frequently
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

  // Whether it's not empty
  inline bool notEmpty()
  {return count != 0;}

  // Function most overloaded(still a pure virtual one)
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame)=0;

  // Paint the icon of the bonus item
  virtual void paintLocatingIcon(QPainter *painter,
                                 QPointF pos,
                                 int frame)=0;

  // Paint the influenced area of the bonus item
  virtual void paintInfluencedArea(int index,
                                   AbstractGameBoardInfo *gameBoard,
                                   EffectPainter *effectPainter,
                                   int frame)=0;

private:
  // 2 key values of a bonus item
  int max;
  int count;
};

// A class of an item of a flame bonus
class FlameItem : public AbstractBonusItem
{
public:
  // Constructor
  FlameItem();

  // Functions most overloaded
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

private:
  // The icon of the item
  QPixmap p;
};

// A class of an item of a star bonus
class StarItem : public AbstractBonusItem
{
public:
  // Constructor
  StarItem();

  // Functions most overloaded
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

private:
  // The icon of the item
  QPixmap p;
};

// A class of an item of a button with "HINT"
class HintItem : public AbstractItem
{
public:
  // Function most overloaded
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);
};

// A class of an item of a button with "EXIT"
class ExitItem : public AbstractItem
{
public:
  // Function most overloaded
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);
};

// A class of an item of a button with "RESET"
class ResetItem : public AbstractItem
{
public:
  // Function most overloaded
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);
};

// A class of an item of a button with "PAUSE"
class PauseItem : public AbstractItem
{
public:
  // Function most overloaded
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);
};

// A class of an item of a button with "CONFIRM"
class ConfirmItem : public AbstractItem
{
public:
  // Function most overloaded
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);
};

// A class of an item of a button with "CANCEL"
class CancelItem : public AbstractItem
{
public:
  // Function most overloaded
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);
};

// A class of an item to show an integer and a string to describe it
class IntegerItem : public AbstractItem
{
public:
  // Constructor
  IntegerItem();

  // Functions to set and get the 2 values
  inline void setHint(QString str)
  {hint = str;}

  inline QString getHint()
  {return hint;}

  inline void setValue(int v)
  {value = v;}

  inline int getValue()
  {return value;}

  // Function most overloaded
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);

private:
  // The background of the item
  QPixmap p;

  // The description of the integer
  QString hint;

  // The integer
  int value;
};

// A class of an item to show a string
class StringItem : public AbstractItem
{
public:
  // Constructor
  StringItem();

  // Functions to set and get the string
  inline void setHint(QString str)
  {hint = str;}

  inline QString getHint()
  {return hint;}

  // Function most overloaded
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame);

private:
  // The background of the item
  QPixmap p;

  // The string
  QString hint;
};

#endif // GAMECOMMONITEMS_H
