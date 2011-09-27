#ifndef HELPITEM_H
#define HELPITEM_H

// File must include
#include "abstractitem.h"
#include <QVector>
#include <QPixmap>
#include <QString>

class HelpItem : public AbstractRectItem
{
public:
  static int totalItems();

  HelpItem(int theIndex);

  virtual void paint(QPainter *painter, int width, int height, int frame);

  virtual double width();
  virtual double height();

  QString mainPartBackGroundPath();
  QVector<QPointF> mainPartAnimCenters();
  QVector<QString> mainPartAnimPaths();

private:
  int index;
};

#endif // HELPITEM_H
