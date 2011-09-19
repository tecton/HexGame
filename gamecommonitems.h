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
  //@{
  /** Set and get the min value of the progress bar. */
  inline void setMin(int v)
  {min = v;}

  inline int getMin()
  {return min;}
  //@}

  //@{
  /** Set and get the max value of the progress bar. */
  inline void setMax(int v)
  {max = v;}

  inline int getMax()
  {return max;}
  //@}

  //@{
  /** Set and get the current value of the progress bar. */
  inline void setCurrent(int v)
  {current = v;}

  inline int getCurrent()
  {return current;}
  //@}

  /**
   * @brief Function most overloaded(still a pure virtual one).
   */
  virtual void paint(QPainter *painter, int width, int height, int frame)=0;

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
  /**
   * @brief Constructor.
   */
  VerticalProgressBarItem();

  /**
   * @brief Function most overloaded.
   */
  virtual void paint(QPainter *painter, int width, int height, int frame);

private:
  // Pixmaps to do the paint
  QPixmap background;
  QPixmap foreground;
};

// An abstract class of an item of a bonus
class AbstractBonusItem : public AbstractItem
{
public:
  //@{
  /** Set and get the current value of the bonus item. */
  inline void setCurrent(int v)
  {
    if (v <= max && v >= 0)
      count = v;
  }

  inline int getCurrent()
  {return count;}
  //@}

  //@{
  /** Set and get the max value of the bonus item. */
  inline void setMax(int v)
  {
    if (v >= 0)
      max = v;
    if (count > max)
      count = max;
  }

  inline int getMax()
  {return max;}
  //@}

  /**
   * @brief Add one and minus one, may be used frequently.
   */
  inline void addOne()
  {
    if (count < max)
      ++count;
  }

  /**
   * @brief Minus one and minus one, may be used frequently.
   */
  inline void minusOne()
  {
    if (count > 0)
      --count;
  }

  /**
   * @brief Whether it's not empty.
   */
  inline bool notEmpty()
  {return count != 0;}

  /**
   * @brief Function most overloaded(still a pure virtual one).
   *
   * @param painter The painter.
   * @param width The width.
   * @param height The height.
   * @param frame The index of the frame to show.
   */
  virtual void paint(QPainter *painter, int width, int height, int frame)=0;

  /**
   * @brief Paint the icon of the bonus item.
   *
   * @param painter The painter.
   * @param pos The center position of the icon.
   * @param frame The index of the frame to show.
   */
  virtual void paintLocatingIcon(QPainter *painter,
                                 int width,
                                 int height,
                                 QPointF pos,
                                 int frame)=0;

  /**
   * @brief Paint the influenced area of the bonus item.
   */
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
  /**
   * @brief Constructor.
   */
  FlameItem();

  //@{
  /** Functions most overloaded. */
  virtual void paint(QPainter *painter, int width, int height, int frame);
  virtual void paintLocatingIcon(QPainter *painter,
                                 int width,
                                 int height,
                                 QPointF pos,
                                 int frame);
  virtual void paintInfluencedArea(int index,
                                   AbstractGameBoardInfo *gameBoard,
                                   EffectPainter *effectPainter,
                                   int frame);
  //@}

private:
  // The icon of the item
  QPixmap p;
};

// A class of an item of a star bonus
class StarItem : public AbstractBonusItem
{
public:
  /**
   * @brief Constructor.
   */
  StarItem();

  //@{
  /** Functions most overloaded. */
  virtual void paint(QPainter *painter, int width, int height, int frame);
  virtual void paintLocatingIcon(QPainter *painter,
                                 int width,
                                 int height,
                                 QPointF pos,
                                 int frame);
  virtual void paintInfluencedArea(int index,
                                   AbstractGameBoardInfo *gameBoard,
                                   EffectPainter *effectPainter,
                                   int frame);
  //@}

private:
  // The icon of the item
  QPixmap p;
};

// A class of an item of a button with "HINT"
class HintItem : public AbstractItem
{
public:
  /**
   * @brief Function most overloaded.
   */
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

// A class of an item of a button with "EXIT"
class ExitItem : public AbstractItem
{
public:
  /**
   * @brief Function most overloaded.
   */
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

// A class of an item of a button with "RESET"
class ResetItem : public AbstractItem
{
public:
  /**
   * @brief Function most overloaded.
   */
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

// A class of an item of a button with "PAUSE"
class PauseItem : public AbstractItem
{
public:
  /**
   * @brief Function most overloaded.
   */
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

// A class of an item of a button with "CONFIRM"
class ConfirmItem : public AbstractItem
{
public:
  /**
   * @brief Function most overloaded.
   */
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

// A class of an item of a button with "CANCEL"
class CancelItem : public AbstractItem
{
public:
  /**
   * @brief Function most overloaded.
   */
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

// A class of an item to show an integer and a string to describe it
class IntegerItem : public AbstractItem
{
public:
  /**
   * @brief Constructor.
   */
  IntegerItem();

  //@{
  /** Set and get the hint of the item. */
  inline void setHint(QString str)
  {hint = str;}

  inline QString getHint()
  {return hint;}
  //@}

  //@{
  /** Set and get the value of the item. */
  inline void setValue(int v)
  {value = v;}

  inline int getValue()
  {return value;}
  //@}

  /**
   * @brief Function most overloaded.
   */
  virtual void paint(QPainter *painter, int width, int height, int frame);

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
  /**
   * @brief Constructor.
   */
  StringItem();

  //@{
  /** Set and get the value of the item. */
  inline void setHint(QString str)
  {hint = str;}

  inline QString getHint()
  {return hint;}
  //@}

  /**
   * @brief Function most overloaded.
   */
  virtual void paint(QPainter *painter, int width, int height, int frame);

private:
  // The background of the item
  QPixmap p;

  // The string
  QString hint;
};

#endif // GAMECOMMONITEMS_H
