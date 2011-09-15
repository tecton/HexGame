/*
** Classes of items used in puzzle stage menu.
*/

#ifndef STAGEMENUITEMS_H
#define STAGEMENUITEMS_H

#include "abstractitem.h"

// File must include
#include <QPointF>
#include <QVector>

// File must include
class QPixmap;

class StageMenuItem : public AbstractItem
{
public:
  StageMenuItem(QString path);

  // The function must be overloaded
  virtual void paint(QPainter *painter, int width, int height, int frame);

private:
  // The position of the item
  // (The position is relative one, NOT absolute one!)
  QPointF position;

  // The pixmap's vector of the item
  QVector<QPixmap> pixmaps;

  // The count of the pixmaps
  int count;
};


#endif // STAGEMENUITEMS_H
