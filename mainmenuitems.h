/*
** Classes of items used in main menu.
*/

#ifndef MAINMENUITEMS_H
#define MAINMENUITEMS_H

#include "abstractitem.h"

// File must include
#include <QPointF>

// Forward declaration
class QPixmap;

class AbstractMainMenuItem : public AbstractItem
{
public:
  // Type of the items(not finished yet)
  enum ItemType{SwapClassicItem=0,
                RotateClassicItem,
                SwapEndlessItem,
                RotateEndlessItem,
                SwapTimingItem,
                RotateTimingItem,
                RotatePuzzleItem,
                HelpItem,
                ExitItem};

  // The function must be overloaded(still pure virtual)
  virtual void paint(QPainter *painter, int width, int height, int frame)=0;

  // A function returns the pixmap of an item
  static const QPixmap& pixmap(ItemType type, int frame);
};


class MainMenuGameItem : public AbstractMainMenuItem
{
public:
  // Constructor with the type of the item
  MainMenuGameItem(AbstractMainMenuItem::ItemType theType) :
      type(theType) {}

  // The function must be overloaded
  virtual void paint(QPainter *painter, int width, int height, int frame);

private:
  // The type of the item
  AbstractMainMenuItem::ItemType type;
};

#endif // MAINMENUITEMS_H
