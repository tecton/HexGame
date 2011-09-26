#ifndef STAGEMENUITEMS_H
#define STAGEMENUITEMS_H

#include "abstractitem.h"

// File must include
#include <QPointF>
#include <QVector>

// File must include
class QPixmap;

/**
 * @brief A class of items used in puzzle stage menu.
 */
class StageMenuItem : public AbstractCircleItem
{
public:
  /**
   * @brief Constructor with the path of the item.
   */
  StageMenuItem(QString path);

  virtual void paint(QPainter *painter, int width, int height, int frame);

  virtual double r();

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
