/*
** Classes of items used in puzzle secondary menu.
*/

#ifndef ABSTRACTPUZZLEMENUITEMS_H
#define ABSTRACTPUZZLEMENUITEMS_H

#include "abstractitem.h"

// File must include
#include <QPointF>

// File must include
class QPixmap;

class AbstractPuzzleMenuItem : public AbstractItem
{
public:
  // Type of the items
  enum ItemType{ExchangeItem=0, UniteItem, LockItem};

  // The function must be overloaded(still pure virtual)
  virtual void paint(QPainter *painter, int width, int height, int frame)=0;

  // A function returns the pixmap of an item
  static const QPixmap& pixmap(ItemType type, int frame);

private:
  // The position of the item
  // (The position is relative one, NOT absolute one!)
  QPointF position;
};

class PuzzleMenuExchangeItem : public AbstractPuzzleMenuItem
{
public:
  // Constructor
  PuzzleMenuExchangeItem();

  // The function must be overloaded
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

class PuzzleMenuUniteItem : public AbstractPuzzleMenuItem
{
public:
  // Constructor
  PuzzleMenuUniteItem();

  // The function must be overloaded
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

class PuzzleMenuLockItem : public AbstractPuzzleMenuItem
{
public:
  // Constructor
  PuzzleMenuLockItem();

  // The function must be overloaded
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

#endif // ABSTRACTPUZZLEMENUITEMS_H
