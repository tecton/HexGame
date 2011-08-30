
#include "stagemenuitems.h"

#include <QVector>
#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QPainter>
#include "pixmapoperations.h"

StageMenuItem::StageMenuItem(QString path)
{
  initPixmaps(path, pixmaps, count);
  setPos(QPointF(0, 0));
}

void StageMenuItem::paint(QPainter *painter,
                          int width,
                          int height,
                          int frame)
{
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  drawPixmapAt(painter,
               pixmaps[frame % count],
               1,
               1,
               QPointF(x, y),
               false,
               true);
}
