/*
** Classes of items used in puzzle secondary menu.
*/

#ifndef ABSTRACTPUZZLEMENUITEMS_H
#define ABSTRACTPUZZLEMENUITEMS_H

#include "abstractitem.h"

#include <QPointF>
class QPixmap;

class AbstractPuzzleMenuItem : public AbstractItem
{
public:
  // Type of the items(not finished yet)
  enum ItemType{ExchangeItem=0, UniteItem};

  // The pure virtual function to be overloaded
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
  PuzzleMenuExchangeItem();
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

class PuzzleMenuUniteItem : public AbstractPuzzleMenuItem
{
public:
  PuzzleMenuUniteItem();
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

class PuzzleMenuStageItem : public AbstractPuzzleMenuItem
{
public:
  PuzzleMenuStageItem();
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

#endif // ABSTRACTPUZZLEMENUITEMS_H
