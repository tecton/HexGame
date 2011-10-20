#ifndef MAINMENUITEMS_H
#define MAINMENUITEMS_H

#include "abstractitem.h"

// File must include
#include <QPointF>
#include <QPixmap>

// Forward declaration
class QPixmap;

/**
 * @brief An abstract class of items used in main menu.
 */
class AbstractMainMenuItem
{
public:
  /**
   * @brief Type of the items.
   */
  enum ItemType{SwapClassicItem=0,
                RotateClassicItem,
                SwapEndlessItem,
                RotateEndlessItem,
                SwapTimingItem,
                RotateTimingItem,
                RotatePuzzleItem,
                HelpItem,
                ExitItem,
                TwoPlayerGame,
                UnvisibleBig,
                };

  /**
   * @brief A function returns the pixmap of an item.
   */
  static const QPixmap& pixmap(ItemType type, int frame);
};

/**
 * @brief A class of items used in main menu which is buttons in a circle.
 */
class MainMenuGameItem  : public AbstractCircleItem
{
public:
  /**
   * @brief Constructor with the type of the item.
   */
  MainMenuGameItem(AbstractMainMenuItem::ItemType theType);

  virtual void paint(QPainter *painter, int width, int height, int frame);

  virtual double r();

private:
  // The type of the item
  AbstractMainMenuItem::ItemType type;
};

/**
 * @brief A class of items used in main menu which is buttons in a rectangle.
 */
class MainMenuButtonItem : public AbstractRectItem
{
public:
  /**
   * @brief Constructor with the type of the item.
   */
  MainMenuButtonItem(AbstractMainMenuItem::ItemType theType) :
      type(theType) {}

  virtual void paint(QPainter *painter, int width, int height, int frame);

  virtual double width();
  virtual double height();

private:
  // The type of the item
  AbstractMainMenuItem::ItemType type;
};

/**
 * @brief A class of item used in main menu which is a rotating circle
 */
class RotatingCircleItem : public AbstractItem
{
public:
  /**
   * @brief Constructor with the type of the item.
   */
  RotatingCircleItem(bool clockwise);

  virtual void paint(QPainter *painter, int width, int height, int frame);
  virtual bool in(QPointF mousePos, int width, int height)
  {return false;}

private:
  QPixmap p;
  bool c;
};

#endif // MAINMENUITEMS_H
