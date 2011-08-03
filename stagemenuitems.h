#ifndef STAGEMENUITEMS_H
#define STAGEMENUITEMS_H

#include "abstractitem.h"

#include <QPointF>
#include <QVector>
class QPixmap;

class StageMenuItem : public AbstractItem
{
public:
  StageMenuItem(QString path, int number);

  // The pure virtual function to be overloaded
  virtual void paint(QPainter *painter, int width, int height, int frame);

private:
  // The position of the item
  // (The position is relative one, NOT absolute one!)
  QPointF position;
  // The pixmap's vector of the item
  QVector<QPixmap> pixmaps;
  int count;
};


#endif // STAGEMENUITEMS_H
