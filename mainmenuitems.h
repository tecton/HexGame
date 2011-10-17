#ifndef MAINMENUITEMS_H
#define MAINMENUITEMS_H

#include "abstractitem.h"

// File must include
#include <QPointF>

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
                TwoPlayerTimingGame};

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
  MainMenuGameItem(AbstractMainMenuItem::ItemType theType) :
      type(theType) {}

  virtual void paint(QPainter *painter, int width, int height, int frame);

  virtual double r();

private:
  // The type of the item
  AbstractMainMenuItem::ItemType type;
};

/**
 * @brief A class of items used in main menu which is buttons in a rectangle.
 */
class MainMenuButtonItem  : public AbstractRectItem
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

#endif // MAINMENUITEMS_H
