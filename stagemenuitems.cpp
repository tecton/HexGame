
#include "stagemenuitems.h"

#include <QVector>
#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QPainter>
#include "pixmapoperations.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

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
  // Calculate the values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  double xRate = 1.0 * width / LOGICAL_WIDTH;
  double yRate = 1.0 * height / LOGICAL_HEIGHT;

  // draw the pixmap
  drawPixmapAt(painter,
               pixmaps[frame % count],
               xRate,
               yRate,
               QPointF(x, y),
               true,
               true);
}
