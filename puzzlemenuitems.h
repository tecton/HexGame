/*
**
*/

#ifndef ABSTRACTPUZZLEMENUITEMS_H
#define ABSTRACTPUZZLEMENUITEMS_H

#include "abstractitem.h"

// File must include
#include <QPointF>

// Forward declaration
class QPixmap;

/**
 * @brief An abstract class of items used in puzzle secondary menu.
 */
class AbstractPuzzleMenuItem : public AbstractRectItem
{
public:
  /**
   * @brief Type of the items.
   */
  enum ItemType{ExchangeItem=0, UniteItem, LockItem};

  virtual void paint(QPainter *painter, int width, int height, int frame)=0;

  /**
   * @brief A function returns the pixmap of an item.
   */
  static const QPixmap& pixmap(ItemType type, int frame);

  virtual double width();
  virtual double height();

private:
  // The position of the item
  // (The position is relative one, NOT absolute one!)
  QPointF position;
};

/**
 * @brief A class of items used in puzzle secondary menu of exchange serial.
 */
class PuzzleMenuExchangeItem : public AbstractPuzzleMenuItem
{
public:
  /**
   * @brief Constructor.
   */
  PuzzleMenuExchangeItem();

  virtual void paint(QPainter *painter, int width, int height, int frame);
};

/**
 * @brief A class of items used in puzzle secondary menu of unit serial.
 */
class PuzzleMenuUniteItem : public AbstractPuzzleMenuItem
{
public:
  /**
   * @brief Constructor.
   */
  PuzzleMenuUniteItem();

  virtual void paint(QPainter *painter, int width, int height, int frame);
};

/**
 * @brief A class of items used in puzzle secondary menu of lock serial.
 */
class PuzzleMenuLockItem : public AbstractPuzzleMenuItem
{
public:
  /**
   * @brief Constructor.
   */
  PuzzleMenuLockItem();

  virtual void paint(QPainter *painter, int width, int height, int frame);
};

#endif // ABSTRACTPUZZLEMENUITEMS_H
