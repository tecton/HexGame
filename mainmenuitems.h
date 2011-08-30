/*
** Classes of items used in main menu.
*/

#ifndef MAINMENUITEMS_H
#define MAINMENUITEMS_H

#include "abstractitem.h"

#include <QPointF>
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
                RotatePuzzleItem};

  // The pure virtual function to be overloaded
  virtual void paint(QPainter *painter, int width, int height, int frame)=0;

  // A function returns the pixmap of an item
  static const QPixmap& pixmap(ItemType type, int frame);
};


class MainMenuSwapClassicItem : public AbstractMainMenuItem
{
public:
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

class MainMenuRotateClassicItem : public AbstractMainMenuItem
{
public:
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

class MainMenuSwapEndlessItem : public AbstractMainMenuItem
{
public:
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

class MainMenuRotateEndlessItem : public AbstractMainMenuItem
{
public:
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

class MainMenuSwapTimingItem : public AbstractMainMenuItem
{
public:
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

class MainMenuRotateTimingItem : public AbstractMainMenuItem
{
public:
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

class MainMenuRotatePuzzleItem : public AbstractMainMenuItem
{
public:
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

#endif // MAINMENUITEMS_H
