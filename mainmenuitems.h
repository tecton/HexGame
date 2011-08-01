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
  enum ItemType{SwapClassicGameItem=0};

  // The pure virtual function to be overloaded
  virtual void paint(QPainter *painter, int width, int height, int frame)=0;

  // A function returns the pixmap of an item
  static const QPixmap& pixmap(ItemType type, int frame);

  // Set the position of the item
  // (The position is relative one, NOT absolute one!)
  void setPos(QPointF pos)
  {position = pos;}


  // Get the position of the item
  // (The position is relative one, NOT absolute one!)
  QPointF getPos()
  {return position;}

private:
  // The position of the item
  // (The position is relative one, NOT absolute one!)
  QPointF position;
};


class MainMenuSwapClassicGameItem : public AbstractMainMenuItem
{
public:
  MainMenuSwapClassicGameItem();
  virtual void paint(QPainter *painter, int width, int height, int frame);
};

#endif // MAINMENUITEMS_H
