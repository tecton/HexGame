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
class AbstractMainMenuItem : public AbstractItem
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
                ExitItem};

  /**
   * @brief Function most overloaded(still a pure virtual one).
   */
  virtual void paint(QPainter *painter, int width, int height, int frame)=0;

  /**
   * @brief A function returns the pixmap of an item.
   */
  static const QPixmap& pixmap(ItemType type, int frame);
};

/**
 * @brief A class of items used in main menu.
 */
class MainMenuGameItem : public AbstractMainMenuItem
{
public:
  /**
   * @brief Constructor with the type of the item.
   */
  MainMenuGameItem(AbstractMainMenuItem::ItemType theType) :
      type(theType) {}

  virtual void paint(QPainter *painter, int width, int height, int frame);

private:
  // The type of the item
  AbstractMainMenuItem::ItemType type;
};

#endif // MAINMENUITEMS_H
