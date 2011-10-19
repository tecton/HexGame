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

/**
 * @brief An abstract class of an item of a progress bar.
 */
class AbstractProgressBarItem : public AbstractItem
{
public:
  /**
   * @brief Set the min value of the progress bar.
   */
  inline void setMin(int v)
  {min = v;}

  /**
   * @brief Get the min value of the progress bar.
   */
  inline int getMin()
  {return min;}

  /**
   * @brief Set the max value of the progress bar.
   */
  inline void setMax(int v)
  {max = v;}

  /**
   * @brief Get the max value of the progress bar.
   */
  inline int getMax()
  {return max;}

  /**
   * @brief Set the current value of the progress bar.
   */
  inline void setCurrent(int v)
  {current = v;}

  /**
   * @brief Get the current value of the progress bar.
   */
  inline int getCurrent()
  {return current;}

  virtual void paint(QPainter *painter, int width, int height, int frame)=0;

  virtual bool in(QPointF mousePos, int windowWidth, int windowHeight)
  {return false;}

private:
  // 3 key values of a progress bar
  int min;
  int max;
  int current;
};

/**
 * @brief A class of an item of a vertical progress bar.
 */
class VerticalProgressBarItem : public AbstractProgressBarItem
{
public:
  /**
   * @brief Constructor.
   */
  VerticalProgressBarItem();

  virtual void paint(QPainter *painter, int width, int height, int frame);

private:
  // Pixmaps to do the paint
  QPixmap background;
  QPixmap foreground;
};

/**
 * @brief An abstract class of an item of a bonus.
 */
class AbstractBonusItem : public AbstractCircleItem
{
public:
  /**
   * @brief Set the current value of the bonus item.
   */
  inline void setCurrent(int v)
  {
    if (v <= max && v >= 0)
      count = v;
  }

  /**
   * @brief Get the current value of the bonus item.
   */
  inline int getCurrent()
  {return count;}

  /**
   * @brief Set the max value of the bonus item.
   */
  inline void setMax(int v)
  {
    if (v >= 0)
      max = v;
    if (count > max)
      count = max;
  }

  /**
   * @brief Get the max value of the bonus item.
   */
  inline int getMax()
  {return max;}

  /**
   * @brief Add one, may be used frequently.
   */
  inline void addOne()
  {
    if (count < max)
      ++count;
  }

  /**
   * @brief Minus one, may be used frequently.
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

  virtual void paint(QPainter *painter, int width, int height, int frame)=0;

  /**
   * @brief Paint the icon of the bonus item.
   *
   * @param painter The painter.
   * @param width Width of the whole window.
   * @param height Height of the whole window.
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
   *
   * @param index The index of the center ball.
   * @param gameBoard The infomation of the gameboard.
   * @param effectPainter The painter to do add effect.
   * @param frame The index of the frame to show.
   */
  virtual void paintInfluencedArea(int index,
                                   AbstractGameBoardInfo *gameBoard,
                                   EffectPainter *effectPainter,
                                   int frame)=0;

  virtual double r();

  virtual void setRotation(int r)
  {rotation = r;}

  inline int getRotation()
  {return rotation;}

private:
  // 2 key values of a bonus item
  int max;
  int count;
  int rotation;
};

/**
 * @brief A class of an item of a flame bonus.
 */
class FlameItem : public AbstractBonusItem
{
public:
  /**
   * @brief Constructor.
   */
  FlameItem();

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
  virtual void setRotation(int r);

private:
  // The icon of the item
  QPixmap p;
};

/**
 * @brief A class of an item of a star bonus.
 */
class StarItem : public AbstractBonusItem
{
public:
  /**
   * @brief Constructor.
   */
  StarItem();

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
  virtual void setRotation(int r);

private:
  // The icon of the item
  QPixmap p;
};

/**
 * @brief A class of an item of a button with text
 */
class ButtonItem : public AbstractRectItem
{
public:
  /**
   * @brief Constructor.
   */
  ButtonItem(QString str);

  virtual void paint(QPainter *painter, int width, int height, int frame);

  virtual double width();
  virtual double height();

  void setRotation(int r);

  inline int getRotation()
  {return rotation;}


private:
  QString caption;
  QPixmap background;
  int rotation;
};

/**
 * @brief A class of an item to show an integer and a string to describe it.
 */
class IntegerItem : public AbstractItem
{
public:
  /**
   * @brief Constructor.
   */
  IntegerItem();

  /**
   * @brief Set the hint of the item.
   */
  inline void setHint(QString str)
  {hint = str;}


  /**
   * @brief Get the hint of the item.
   */
  inline QString getHint()
  {return hint;}

  /**
   * @brief Set the value of the item.
   */
  inline void setValue(int v)
  {value = v;}

  /**
   * @brief Get the value of the item.
   */
  inline int getValue()
  {return value;}

  virtual void paint(QPainter *painter, int width, int height, int frame);

  virtual bool in(QPointF mousePos, int windowWidth, int windowHeight)
  {return false;}

  void setRotation(int r);

  inline int getRotation()
  {return rotation;}

private:
  // The background of the item
  QPixmap p;

  // The description of the integer
  QString hint;

  // The integer
  int value;

  int rotation;
};

/**
 * @brief A class of an item to show a string.
 */
class StringItem : public AbstractItem
{
public:
  /**
   * @brief Constructor.
   */
  StringItem();

  /**
   * @brief Set the value of the item.
   */
  inline void setHint(QString str)
  {hint = str;}

  /**
   * @brief Get the value of the item.
   */
  inline QString getHint()
  {return hint;}

  virtual void paint(QPainter *painter, int width, int height, int frame);

  virtual bool in(QPointF mousePos, int windowWidth, int windowHeight)
  {return false;}

private:
  // The background of the item
  QPixmap p;

  // The string
  QString hint;
};

#endif // GAMECOMMONITEMS_H
